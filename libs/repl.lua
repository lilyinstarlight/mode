-- inspired by a few ideas at https://github.com/hoelzro/lua-repl

local context = {}
for idx, val in pairs(_G) do
	if idx ~= 'context' and idx ~= 'debug' then
		context[idx] = val
	end
end

local buf = ''

function gather (status, ...)
	return status, {n = select('#', ...), ...}
end

function setenv (fun, env)
	local idx = 1
	while true do
		local name = debug.getupvalue(fun, idx)

		if name == '_ENV' then
			debug.setupvalue(fun, idx, env)
			break
		elseif not name then
			break
		end

		idx = idx + 1
	end
end

function repr (val, indent, start)
	indent = indent or 0
	start = start or false

	local typ = type(val)

	if typ == 'string' then
		return string.format('%q', val)
	elseif typ == 'table' then
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

			for tidx, tval in pairs(val) do
				str = str .. string.rep('  ', indent + 1) .. tidx .. ' = ' .. repr(tval, indent + 1) .. ',\n'
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

function repl (chunk)
	buf = buf .. chunk

	fun, err = load('return ' .. buf, 'repl')
	if not fun then
		fun, err = load(buf, 'repl')
	end

	if fun then
		buf = ''

		local result = ''

		setenv(fun, context)

		status, results = gather(xpcall(fun, function(...) return debug.traceback(...) end))
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
