package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public class Balloon extends Aircraft {

	public Balloon(long p_id, String p_name, Coordinates p_coordinate) throws InvalidCoordinatesException {
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
				longitude = (longitude + 2) % 360;
				height = Math.min(height + 4, 100);

				if (longitude == 0)
					longitude = 360;
				break;
			case "RAIN":
				height = Math.max(height - 5, 0);
				break;
			case "FOG":
				height = Math.max(height - 3, 0);
				break;
			case "SNOW":
				height = Math.max(height - 15, 0);
				break;
		}

		updateCoordinates(longitude, latitude, height);

		String updatedWeather = weatherTower.getWeather(coordinates);

		if (!updatedWeather.equals(weather))
			WeatherProvider.getInstance().setChanged();
	}

	@Override
	public void log() {
		log("Let's enjoy the good weather and take some pics.",
				"Damn you rain! You messed up my balloon.",
				"It's so foggy I can't see anything.",
				"It's snowing. We're gonna crash.");
	}
}
