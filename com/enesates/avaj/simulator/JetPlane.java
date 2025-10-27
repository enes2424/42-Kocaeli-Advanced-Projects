package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public class JetPlane extends Aircraft {

	public JetPlane(long p_id, String p_name, Coordinates p_coordinate) throws InvalidCoordinatesException {
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
				latitude = Math.min(latitude + 10, 90);
				height = Math.min(height + 2, 100);
				break;
			case "RAIN":
				latitude = Math.min(latitude + 5, 90);
				break;
			case "FOG":
				latitude = Math.min(latitude + 1, 90);
				break;
			case "SNOW":
				height = Math.max(height - 7, 0);
				break;
		}

		updateCoordinates(longitude, latitude, height);

		String updatedWeather = weatherTower.getWeather(coordinates);

		if (!updatedWeather.equals(weather))
			WeatherProvider.getInstance().setChanged();
	}

	@Override
	public void log() {
		log("It's so sunny!",
				"It's raining. Better watch out for lightings.",
				"It's foggy. I can't see anything.",
				"OMG! Winter is coming!");
	}
}
