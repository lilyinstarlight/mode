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
end

function observe (other)
	if other.type == 'player' then
		local distance = other.pos.x + other.width/2 -  sprite.pos.x - sprite.width/2
		sprite.vel.x = distance/10
	end
end

function collide (other)
	collided = true
end

player:observe()
world:add_dialog(collision)
