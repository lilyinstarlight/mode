speed = {
	left = spec:get_float('player/speed/left'),
	right = spec:get_float('player/speed/right'),
	up = spec:get_float('player/speed/up'),
	down = spec:get_float('player/speed/down')
}

glider = {
	offset = {
		x = spec:get_int('player/glider/offset/x'),
		y = spec:get_int('player/glider/offset/y')
	},
	speed = spec:get_int('player/glider/speed')
}

land = 200

function dispatch (event)
	if event.ev == 'keydown' and event.val.rep == 0 then
		if input:check('player') then
			if event.val.key == 'q' then
				engine:stop()
			elseif event.val.key == 'r' then
				engine:restart()
			elseif event.val.key == 'h' then
				hud:toggle()
			elseif event.val.key == ' ' then
				player:shoot()
			end
		end
	end
end

function update (ticks)
	if ground == nil then
		ground = world:get_background('ground')
	end

	bottom = ground.pos.y - sprite.height
	grounded = sprite.pos.y >= bottom and sprite.vel.y >= 0
	landing = not grounded and sprite.pos.y >= bottom - land and sprite.vel.y >= 0

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

	if sprite.vel.x < 0 or sprite.vel.x > 0 then
		sprite.state = 'run'
	else
		sprite.state = 'idle'
	end

	if grounded then
		sprite.vel.y = 0
		sprite.pos.y = bottom

		if input:check('player') and input:get_key('w') then
			sprite.vel.y = sprite.vel.y - speed.up

			sprite:clear()
			sprite:push('jump')
		end
	end

	if landing then
		sprite:push('land')
	end
end
