function dispatch (event)
end

function update (ticks)
	ground_left, hit_left = world:cast(Vector.new(sprite.pos.x, sprite.pos.y + sprite.height), 3*3.14159/2)
	ground_right, hit_right = world:cast(Vector.new(sprite.pos.x + sprite.width, sprite.pos.y + sprite.height), 3*3.14159/2)

	if ground_left and ground_left.type == 'platform' then
		bottom = ground_left.pos.y - sprite.height
	elseif ground_right and ground_right.type == 'platform' then
		bottom = ground_right.pos.y - sprite.height
	else
		bottom = world.height - sprite.height
	end

	grounded = sprite.pos.y >= bottom and sprite.vel.y >= 0

	sprite.vel.y = sprite.vel.y + ticks

	if grounded then
		sprite.vel.y = 0
		sprite.pos.y = bottom
	end
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
