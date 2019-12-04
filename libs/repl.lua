-- inspired by a few ideas at https://github.com/hoelzro/lua-repl

local context = {}
for idx, val in pairs(_G) do
	if idx == '_G' then
		context[idx] = context
	else
		context[idx] = val
	end
end

local buf = ''

local function traceback (...)
	local thread = nil
	local message = nil
	local level = nil

	if select('#', ...) > 0 then
		if type(select(1, ...)) == 'thread' then
			thread, message, level = select(1, ...)
		else
			message, level = select(1, ...)
		end
	end

	if type(message) ~= 'nil' and type(message) ~= 'string' then
		return message
	end

	if not level then
		if not thread or thread == coroutine.running() then
			level = 2
		else
			level = 0
		end
	end

	local bt = ''

	if message then
		bt = bt .. message .. '\n'
	end

	bt = bt .. 'stack traceback:'

	local llevel = level
	while true do
		local info
		if thread then
			info = debug.getinfo(thread, llevel, "Slntf")
		else
			info = debug.getinfo(llevel, "Slntf")
		end

		if not info or info.func == repl then
			break
		end

		bt = bt .. string.format('\n\t%s:', info.short_src)
		if info.currentline > 0 then
			bt = bt .. string.format('%d:', info.currentline)
		end

		bt = bt .. ' in '

		local name = nil
		for idx, val in pairs(context) do
			if val == info.func then
				name = idx
				break
			end
		end

		if name then
			bt = bt .. string.format('function \'%s\'', name)
		elseif info.namewhat ~= '' then
			bt = bt .. string.format('%s \'%s\'', info.namewhat, info.name)
		elseif info.what == 'm' then
			bt = bt .. 'main chunk'
		elseif info.what ~= 'C' then
			if info.short_src == '[repl]' and info.linedefined == 0 then
				bt = bt .. 'interactive chunk'
			else
				bt = bt .. string.format('function <%s:%d>', info.short_src, info.linedefined)
			end
		else
			bt = bt .. '?'
		end

		if info.istailcall then
			bt = bt .. '\n\t(...tail calls...)'
		end

		llevel = llevel + 1
	end

	return bt
end

local function gather (status, ...)
	return status, {n = select('#', ...), ...}
end

local function repr (val, indent, path, seen)
	indent = indent or 0
	start = start or false
	path = path or '<top>'
	seen = seen or {}

	local typ = type(val)

	if typ == 'string' then
		return string.format('%q', val)
	elseif typ == 'table' then
		if seen[val] then
			return seen[val]
		end

		seen[val] = path

		local str

		if next(val) == nil then
			str = '{}'
		else
			if start then
				str = string.rep('  ', indent)
			else
				str = ''
			end

			str = str .. '{\n'

			for tidx, tval in ipairs(val) do
				str = str .. string.rep('  ', indent + 1) .. tidx .. ' = ' .. repr(tval, indent + 1, path .. '[' .. repr(tidx) .. ']', seen) .. ',\n'
			end

			for tidx, tval in pairs(val) do
				if not (type(tidx) == 'number' and tidx > 0 and tidx <= #val) then
					local tpath
					if type(tidx) == 'string' then
						tpath = path .. '.' .. tidx
					else
						tpath = path .. '[' .. repr(tidx) .. ']'
					end

					str = str .. string.rep('  ', indent + 1) .. tidx .. ' = ' .. repr(tval, indent + 1, tpath, seen) .. ',\n'
				end
			end

			str = str .. string.rep('  ', indent) .. '}'
		end

		return str
	elseif typ == 'userdata' and val.to_string then
		return val:to_string()
	else
		return tostring(val)
	end
end

local function repl (chunk)
	buf = buf .. chunk

	fun, err = load('return ' .. buf, '=[repl]', 't', context)
	if not fun then
		fun, err = load(buf, '=[repl]', 't', context)
	end

	if fun then
		buf = ''

		local result = ''

		local status, results = gather(xpcall(fun, function(...) return traceback(...) end))
		if status then
			if results.n == 0 then
				return 0, nil
			elseif results.n == 1 then
				result = result .. repr(results[1])
			else
				result = result .. '{\n'
				for i = 1, results.n do
					result = result .. '  '
					result = result .. repr(results[i], 1)
					result = result .. ',\n'
				end
				result = result .. '}'
			end
		else
			result = result .. results[1]
		end

		return 0, result
	else
		if string.match(err, '\'<eof>\'$') or string.match(err, '<eof>$') then
			buf = buf .. '\n'

			return 2, nil
		else
			buf = ''

			return 1, err
		end
	end
end

return repl
