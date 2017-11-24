function dispatch (event)
end

function update (ticks)
	if collided then
		collision:open()
	else
		collision:close()
	end

	collided = false

	sprite.vel.y = sprite.vel.y + ticks
end

function observe (sprite)
end

function collide (sprite)
	collided = true
end

player:observe()

collided = false
collision = Dialog.new("collision", "Collision detected!", true, false)
world:add_dialog(collision)
