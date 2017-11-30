max = spec:get_float('glider/max'),

function dispatch (event)
end

function update (ticks)
	if math.sqrt(math.pow(sprite.pos.x - projectile.origin.x, 2) + math.pow(sprite.pos.y - projectile.origin.y, 2)) > max then
		projectile:destroy()
	end
end
