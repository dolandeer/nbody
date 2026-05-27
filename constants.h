#pragma once
//physics constants
constexpr double G_CONSTANT_REGULAR = 6.67300E-11; // m^3/(kgs^2)
constexpr double G_CONSTANT = 39.4784176044; // 4pi^2 in AU^3/(SM·yr^2)
constexpr double DAY = 1.0 / 365.25;


//mass constants
constexpr double EARTH_MASS_CONSTANT_KG = 5.97219E24; // Kilograms
constexpr double SUN_MASS_CONSTANT_KG = 1.989E30; // Kilograms
constexpr double MOON_MASS_CONSTANT_KG = 7.34767309E22; // Kilograms

constexpr double EARTH_MASS_CONSTANT = 3.00260935E-6; // Solar Masses
constexpr double SUN_MASS_CONSTANT = 1; // Solar Masses
constexpr double MOON_MASS_CONSTANT = 3.69415439E-8; // Solar Masses

// radius constants;  equatorial radius approximation
constexpr double EARTH_RADIUS_CONSTANT = 0.0000426354; // Astronomical Units
constexpr double SUN_RADIUS_CONSTANT = 0.0046524726;   // Astronomical Units
constexpr double MOON_RADIUS_CONSTANT = 2.322226E-5; // Astronomical Units

// velocity constants
constexpr double EARTH_ORBIT_V = 6.278697964; // AU/yr
constexpr double MOON_ORBIT_V = 0.2156622459; // AU/yr relative to earth

// distance constants
constexpr double AU = 1.496E11; // meters
// radius constants
constexpr double EARTH_ORBIT_RADIUS = 1.0; // Astronomical Units
constexpr double MOON_ORBIT_RADIUS = 0.00257; // Astronomical Units


//config constants
constexpr double DAY_PER_SECOND = DAY / 60; // based off of maximum framerate, not perfect
constexpr int TRAIL_LENGTH = 365; // steps
