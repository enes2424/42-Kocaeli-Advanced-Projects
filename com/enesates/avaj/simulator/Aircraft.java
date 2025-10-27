package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public abstract class Aircraft extends Flyable {
	protected long id;
	protected String name;
	protected Coordinates coordinates;

	protected Aircraft(long p_id, String p_name, Coordinates p_coordinate) throws InvalidCoordinatesException {
		this.id = p_id;
		this.name = p_name;
		this.coordinates = p_coordinate;
	}

	public String getIdentifier() {
		return this.getClass().getSimpleName() + "#" + this.name + "(" + this.id + ")";
	}

	protected void updateCoordinates(int p_longitude, int p_latitude, int p_height) {
		try {
			this.coordinates = new Coordinates(p_longitude, p_latitude, p_height);
		} catch (InvalidCoordinatesException e) {
			SimulationWriter.getInstance()
					.println(this.getIdentifier() + ": Warning - Invalid coordinates attempted: " + e.getMessage());
		}
	}

	public boolean hasLanded() {
		return this.coordinates.getHeight() <= 0;
	}

	protected void log(String sunMessage, String rainMessage, String fogMessage, String snowMessage) {

		String weather = WeatherProvider.getInstance().getCurrentWeather(this.coordinates);

		String message = switch (weather) {
			case "SUN" -> sunMessage;
			case "RAIN" -> rainMessage;
			case "FOG" -> fogMessage;
			case "SNOW" -> snowMessage;
			default -> "Unknown weather condition.";
		};

		SimulationWriter.getInstance().println(this.getIdentifier() + ": " + message);
	}

	public abstract void log();

}
