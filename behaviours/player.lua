speed = {
	left = spec:get_float('player/speed/left'),
	right = spec:get_float('player/speed/right'),
	up = spec:get_float('player/speed/up'),
	down = spec:get_float('player/speed/down')
}

glider = {
	offset = {
		x = spec:get_int('player/projectile/offset/x'),
		y = spec:get_int('player/projectile/offset/y')
	},
	speed = spec:get_int('player/projectile/speed')
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
	ground_left, hit_left = world:cast(Vector.new(sprite.pos.x, sprite.pos.y + sprite.height), 3*3.14159/2, 'platform')
	ground_right, hit_right = world:cast(Vector.new(sprite.pos.x + sprite.width, sprite.pos.y + sprite.height), 3*3.14159/2, 'platform')

	if ground_left and ground_right then
		if hit_left.y < hit_right.y then
			ground = ground_left
		else
			ground = ground_right
		end
		bottom = ground.pos.y
	elseif ground_left then
		ground = ground_left
		bottom = ground.pos.y
	elseif ground_right then
		ground = ground_right
		bottom = ground.pos.y
	else
		ground = {height = 0}
		bottom = world.height
	end

	ceiling_left, hit_left = world:cast(Vector.new(sprite.pos.x, sprite.pos.y + sprite.height), 3.14159/2, 'platform')
	ceiling_right, hit_right = world:cast(Vector.new(sprite.pos.x + sprite.width, sprite.pos.y + sprite.height), 3.14159/2, 'platform')

	if ceiling_left and ceiling_left.type == 'platform' then
		ceiling = ceiling_left
		top = ceiling.pos.y + ceiling.height
	elseif ceiling_right and ceiling_right.type == 'platform' then
		ceiling = ceiling_right
		top = ceiling.pos.y + ceiling.height
	else
		ceiling = {height = 0}
		top = 0
	end

	grounded = sprite.pos.y - bottom + sprite.height >= 0 and sprite.pos.y - bottom + sprite.height <= ground.height and sprite.vel.y >= 0
	ceiled = top - sprite.pos.y >= 0 and top - sprite.pos.y <= ceiling.height and sprite.vel.y < 0

	landing = not grounded and sprite.pos.y >= bottom - sprite.height - land*sprite.vel.y and sprite.vel.y >= 0

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
