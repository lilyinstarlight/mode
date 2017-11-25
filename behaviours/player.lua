speed = {
	left = spec:get_float('player/speed/left'),
	right = spec:get_float('player/speed/right'),
	up = spec:get_float('player/speed/up'),
	down = spec:get_float('player/speed/down')
}

function dispatch (event)
	if event.ev == 'keydown' and event.val.rep == 0 then
		if input:check('player') then
			if event.val.key == 'q' then
				engine:stop()
			elseif event.val.key == 'r' then
				engine:restart()
			elseif event.val.key == 'h' then
				hud:toggle()
			end
		end
	end
end

function update (ticks)
	sprite.vel.x = 0

	if input:check('player') then
		if input:get_key('a') then
			sprite.vel.x = sprite.vel.x - speed.left
		end

		if input:get_key('d') then
			sprite.vel.x = sprite.vel.x + speed.right
		end

		if input:get_key('s') then
			sprite.vel.y = sprite.vel.y + speed.down
		end
	end

	sprite.vel.y = sprite.vel.y + ticks

	if sprite.pos.y >= world.height - sprite.height - 50 then
		sprite.pos.y = world.height - sprite.height - 50
		sprite.vel.y = 0

		if input:check('player') and input:get_key('w') then
			sprite.vel.y = sprite.vel.y - speed.up
		end
	end

	if sprite.vel.x < 0 then
		sprite.state = 'left'
	elseif sprite.vel.x > 0 then
		sprite.state = 'right'
	end
end
