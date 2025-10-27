package com.enesates.avaj.simulator;

public class WeatherProvider {

	private static WeatherProvider instance = null;
	private static String[] weather = { "RAIN", "FOG", "SUN", "SNOW" };

	private boolean hasChanged = false;

	private WeatherProvider() {
	}

	public static WeatherProvider getInstance() {
		if (instance == null)
			instance = new WeatherProvider();

		return instance;
	}

	public String getCurrentWeather(Coordinates p_coordinates) {
		int index = (int) (Math.pow(p_coordinates.getLongitude(), 2) + Math.exp(p_coordinates.getLatitude())
				+ Math.sqrt(p_coordinates.getHeight())) % 4;

		return weather[index];
	}

	public boolean hasChanged() {
		boolean temp = this.hasChanged;
		this.hasChanged = false;
		return temp;
	}

	public void setChanged() {
		this.hasChanged = true;
	}

}
