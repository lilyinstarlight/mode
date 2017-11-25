collided = false
collision = Dialog.new("collision", "Collision detected!", true, false)
center = sprite.pos.y

function dispatch (event)
end

function update (ticks)
	if collided then
		collision:open()
	else
		collision:close()
	end

	collided = false

	sprite.pos.y = 20*math.sin(clock.ticks/500) + center

	if sprite.vel.x < 0 then
		sprite.state = 'left'
	else
		sprite.state = 'right'
	end
end

function observe (other)
	local distance = sprite.pos.x + sprite.width/2 - other.pos.x - other.width/2

	sprite.vel.x = -distance/10
end

function collide (other)
	collided = true
end

player:observe()
world:add_dialog(collision)
