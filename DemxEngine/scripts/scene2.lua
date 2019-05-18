-- script

for i = 0, 360, 36 do
	createCylinder( "name" .. i, "marble_mat", 0.5, 4, 1, 8 )
	--createModelEntity( "name" ..i, "statue" )
	translate( "name" .. i, math.cos( math.rad( i ) ) * 6, 0, math.sin( math.rad( i ) ) * 6 )
end

--for i = 0, 20 do
--	createModelEntity( "entity", "statue" )
--	translate( "entity", 4, 1, 0 )
--	rotate( "entity", 45, 0, 1, 0 )
--	scaleAll( "entity", 2 )
--end


