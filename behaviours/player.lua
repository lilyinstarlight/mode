speed = {
	left = spec:get_float('player/speed/left'),
	right = spec:get_float('player/speed/right'),
	up = spec:get_float('player/speed/up'),
	down = spec:get_float('player/speed/down')
}

grounded = false

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
	grounded = sprite.pos.y == view.height - sprite.height

	if input:check('player') then
		sprite.vel.x = 0

		if input:get_key('a') then
			sprite.vel.x = sprite.vel.x - speed.left
		end

		if input:get_key('d') then
			sprite.vel.x = sprite.vel.x + speed.right
		end

		if input:get_key('w') and grounded then
			sprite.vel.y = sprite.vel.y - speed.up
		end

		if input:get_key('s') then
			sprite.vel.y = sprite.vel.y + speed.down
		end

		sprite.vel.y = sprite.vel.y + ticks
	end
end
