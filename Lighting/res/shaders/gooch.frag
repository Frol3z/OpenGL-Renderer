	/*
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPosition);
		vec3 viewDir = normalize(vec3(0.0) - FragPosition);
		vec3 baseColor = material.ambient;
	
		vec3 cold = vec3(0, 0, 0.55) + baseColor;
		vec3 warm = vec3(0.3, 0.3, 0) + baseColor;
		vec3 highlight = light.specular;

		float t = (dot(norm, lightDir) + 1) / 2;
		float t1 = max(dot(norm, lightDir), 0);
	
		vec3 color = mix(cold, warm, t);
	
		vec3 r = reflect(-lightDir, norm);
		float s = clamp((100 * dot(r, viewDir) - 97), 0.0, 1.0);
		vec3 result = mix(color, highlight, s);

		FragColor = vec4(result, 1.0);
	*/