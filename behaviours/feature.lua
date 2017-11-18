function update (ticks)
	sprite.vel_y = sprite.vel_y + ticks*80/1000
end

function observe (sprite)
	print('observe')
end

function collide (sprite)
	print('collision')
end

--player.observe()
