collided = false
collision = Dialog.new("collision", "Collision detected!", true, false)
center = sprite.pos.y

land = 200

function dispatch (event)
end

function update (ticks)
	bottom = ground.pos.y - sprite.height
	grounded = sprite.pos.y >= bottom and sprite.vel.y >= 0
	landing = not grounded and sprite.pos.y >= bottom - land and sprite.vel.y >= 0

	if collided then
		collision:open()
	else
		collision:close()
	end

	collided = false

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

	if other.type == 'player' then
		other:kill()
	end
end

ground = world:get_background('ground')

player:observe()
world:add_dialog(collision)
