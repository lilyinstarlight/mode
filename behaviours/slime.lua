collided = false
collision = Dialog.new("collision", "Collision detected!", true, false)
center = sprite.pos.y

land = 200
left = false

function dispatch (event)
end

function update (ticks)
	if ground == nil then
		ground = world:get_background('ground')
	end

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

	if sprite.vel.x < 0 then
		sprite.state = 'idle.left'
	elseif sprite.vel.x > 0 then
		sprite.state = 'idle.right'
	end

	if grounded then
		sprite.vel.y = 0
		sprite.pos.y = bottom
	end
end

function observe (other)
end

function collide (other)
	collided = true
end

player:observe()
world:add_dialog(collision)
