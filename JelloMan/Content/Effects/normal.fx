float3 CalcNormal(float3 normal, float3 tangent, float3 rgb)
{
	//NormalMap
	tangent = normalize(tangent);
	normal = normalize(normal);
	float dottn = dot(tangent,normal);
	tangent -= dottn * normal;
	float3 binormal = normalize(cross(tangent,normal));

	float3x3 assenstelsel = float3x3(binormal,tangent,normal);

	float3 xyz = float3(rgb.x * 2 - 1, rgb.y * 2 - 1, rgb.z * 2 - 1) ;

	return normalize(mul(float4(xyz, 0), assenstelsel));
}