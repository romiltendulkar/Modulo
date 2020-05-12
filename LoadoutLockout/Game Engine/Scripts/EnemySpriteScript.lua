Espritechange = function (EnemyType, Direction)
	spritestring = "Enemy1.png"
	if EnemyType == 0
	then
		if Direction == 0
		then
			spritestring = "Enemy1.png"
		end
		if Direction == 1
		then
			spritestring = "Enemy1.png"		
		end
		if Direction == 2
		then
			spritestring = "Enemy1.png"		
		end
	end
	return spritestring
end
