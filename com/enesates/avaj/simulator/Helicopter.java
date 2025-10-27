package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public class Helicopter extends Aircraft {

	public Helicopter(long p_id, String p_name, Coordinates p_coordinate) throws InvalidCoordinatesException {
		super(p_id, p_name, p_coordinate);
	}

	@Override
	public void updateConditions() {
		String weather = weatherTower.getWeather(coordinates);
		int longitude = coordinates.getLongitude();
		int latitude = coordinates.getLatitude();
		int height = coordinates.getHeight();

		switch (weather) {
			case "SUN":
				longitude = (longitude + 10) % 360;
				height = Math.min(height + 2, 100);
				break;
			case "RAIN":
				longitude = (longitude + 5) % 360;
				break;
			case "FOG":
				longitude = (longitude + 1) % 360;
				break;
			case "SNOW":
				height = Math.max(height - 12, 0);
				break;
		}

		if (longitude == 0)
			longitude = 360;

		updateCoordinates(longitude, latitude, height);

		String updatedWeather = weatherTower.getWeather(coordinates);

		if (!updatedWeather.equals(weather))
			WeatherProvider.getInstance().setChanged();
	}

	@Override
	public void log() {
		log("This is hot.",
				"It's raining. Better watch out for lightings.",
				"It's so foggy I can't see anything.",
				"My rotor is going to freeze!");
	}
}
