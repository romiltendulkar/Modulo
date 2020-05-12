EAnimScript = function(AnimationType, Counter)
{
	NewCounter = Counter - 1;
	SpriteFile = "EnemyR.png";
	if AnimationType == 1
	then
		if Counter == 1
		then
			SpriteFile = "EnemyR.png";		
		end
	end
	return SpriteFile, NewCounter
}