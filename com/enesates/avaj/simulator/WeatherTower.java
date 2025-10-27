package com.enesates.avaj.simulator;

public class WeatherTower extends Tower {

	public String getWeather(Coordinates p_coordinates) {
		return WeatherProvider.getInstance().getCurrentWeather(p_coordinates);
	}

	public void changeWeather() {
		conditionChanged();

		if (WeatherProvider.getInstance().hasChanged())
			logObservers();
	}

}
