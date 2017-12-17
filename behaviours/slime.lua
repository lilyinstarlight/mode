function dispatch (event)
end

function update (ticks)
	bottom = ground.pos.y - sprite.height
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
