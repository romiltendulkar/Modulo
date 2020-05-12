
return1 = function()
	return 1
end

moveamount = .65;

move = function(XString, YString, dt, SPEED)
	x = 0;
	y = 0;
	if XString == "LEFT"
	then 
		x = -moveamount * SPEED/2
	end
	if XString == "RIGHT"
	then 
		x = moveamount * SPEED/2
	end
	--if YString == "DOWN"
	--then 
		--y = -moveamount * SPEED
	--end
	if YString == "UP"
	then 
		y = moveamount * SPEED* 2
	end
	return x, y
end