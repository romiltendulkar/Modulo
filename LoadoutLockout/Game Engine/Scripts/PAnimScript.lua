animscript = function(AnimationType, Counter)
{

	SpriteFile = "RobotStill.png";
	if AnimationType == 1
	then
		if Counter < 300
		then
			SpriteFile = "D_Frame1.png";		
		end
		if Counter < 240
		then
			SpriteFile = "D_Frame2.png";		
		end
		if Counter < 180
		then
			SpriteFile = "D_Frame3.png";
		end
		if Counter < 120
		then
			SpriteFile = "D_Frame4.png";
		end
	end
	return SpriteFile
}