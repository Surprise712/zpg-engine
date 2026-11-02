#version 330 core

in vec3 vertexNormal;
in vec3 baseColor;
in vec4 worldPosition; // DŮLEŽITÉ: Toto musíš přidat a poslat z vertex shaderu!

out vec4 FragColor;

uniform vec3 lightPosition; // Zde z C++ pošleš pozici světlušky

void main()
{
    // 1. Ambientní světlo - noční les je temný, takže jen slabé modro-šedé světlo,
    // aby stromy v dálce nebyly úplně 100% černé.
    vec3 ambient = vec3(0.02, 0.02, 0.05) * baseColor;

    // 2. Difúzní složka (jak moc světluška osvětluje dané místo)
    vec3 N = normalize(vertexNormal);
    vec3 L = normalize(lightPosition - worldPosition.xyz);
    
    float diff = max(dot(N, L), 0.0);
    
    // Světluška typicky svítí teple žlutozeleně
    vec3 lightColor = vec3(0.8, 1.0, 0.4); 
    vec3 diffuse = diff * baseColor * lightColor;

    // 3. Výpočet útlumu (Attenuation) - Bod 20 ze zadání
    float distance = length(lightPosition - worldPosition.xyz);
    
    // Parametry pro dosah světla (tyhle hodnoty dělají hezký plynulý dosah cca 5-10 metrů)
    float constant = 1.0;
    float linear = 0.14;
    float quadratic = 0.07; 

    // Rovnice pro útlum
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Útlum aplikujeme pouze na difúzní světlo od světlušky (ambient necháme, ať je vidět silueta lesa)
    diffuse *= attenuation;

    // Sečteme temný základ lesa a osvětlení od světlušky
    vec3 finalColor = ambient + diffuse;

    FragColor = vec4(finalColor, 1.0);
}