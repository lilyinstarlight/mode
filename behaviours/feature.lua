center = sprite.pos.y

win = Dialog.new("win", "", true, false)

function dispatch (event)
end

function update (ticks)
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

	if other.name == 'player' then
		other:kill()
	elseif other.name == 'glider' then
		other:kill()
		sprite:inject()
	end
end

player:observe()
world:add_dialog(win)
