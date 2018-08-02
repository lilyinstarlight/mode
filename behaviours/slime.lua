function dispatch (event)
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

	grounded = sprite.pos.y - bottom + sprite.height >= 0 and sprite.pos.y - bottom + sprite.height <= ground.height and sprite.vel.y >= 0

	sprite.vel.y = sprite.vel.y + ticks
end

function observe (other)
end

function collide (other)
	collided = true

	if other.name == 'player' then
		other:kill()
	elseif other.name == 'glider' then
		other:kill()
		sprite:inject()
	end
end

ground = world:get_background('ground')

player:observe()
