speed_left = spec:get_float('player/speed/left')
speed_right = spec:get_float('player/speed/right')
speed_up = spec:get_float('player/speed/up')
speed_down = spec:get_float('player/speed/down')

function dispatch (event)
	if event.type == 'keydown' then
		if input:check('player') then
			if event.val['key'] == 'q' then
				engine:stop()
			elseif event.val['key'] == 'h' then
				hud:toggle()
			end
		end
	end
end

function update (ticks)
	if input:check('player') then
		sprite.vel.x = 0
		sprite.vel.y = 0

		if input:get_key('a') then
			sprite.vel.x = sprite.vel.x - speed_left
		end

		if input:get_key('d') then
			sprite.vel.x = sprite.vel.x + speed_right
		end

		if input:get_key('w') then
			sprite.vel.y = sprite.vel.y - speed_up
		end

		if input:get_key('s') then
			sprite.vel.y = sprite.vel.y + speed_down
		end
	end
end
