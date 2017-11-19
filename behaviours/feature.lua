function update (ticks)
	sprite.vel.y = sprite.vel.y + ticks*80/1000
end

function observe (sprite)
end

function collide (sprite)
end

player:observe()
