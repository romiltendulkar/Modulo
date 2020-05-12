spritechange = function(Equipment_Enum, Direction_Enum)
	SpriteString = "RobotStill.png";
	if Equipment_Enum == 0
	then
		if Direction_Enum == 0
		then
			SpriteString = "RobotStill.png"
		end
		if Direction_Enum == 1
		then
			SpriteString = "RobotLeft.png"
		end
		if Direction_Enum == 2
		then
			SpriteString = "RobotRight.png"
		end
	end
	if Equipment_Enum == 1
	then
		if Direction_Enum == 0
		then
			SpriteString = "JumpAnimation.png"
		end
		if Direction_Enum == 1
		then
			SpriteString = "JumpLeft.png"
		end
		if Direction_Enum == 2
		then
			SpriteString = "JumpRight.png"
		end
	end
	return SpriteString
end