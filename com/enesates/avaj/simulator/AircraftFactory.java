package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidAircraftTypeException;
import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public class AircraftFactory {

	private static AircraftFactory aircraftFactory = null;
	private int idCounter = 1;

	private AircraftFactory() {
	}

	public static AircraftFactory getInstance() {
		if (aircraftFactory == null)
			aircraftFactory = new AircraftFactory();

		return aircraftFactory;
	}

	public Flyable newAircraft(String p_type, String p_name, Coordinates p_coordinates)
			throws InvalidAircraftTypeException, InvalidCoordinatesException {
		if (p_type == null || p_name == null || p_coordinates == null)
			throw new InvalidAircraftTypeException("Aircraft type, name, and coordinates cannot be null");

		if (p_name.trim().isEmpty())
			throw new InvalidAircraftTypeException("Aircraft name cannot be empty");

		switch (p_type) {
			case "Balloon":
				return new Balloon(idCounter++, p_name, p_coordinates);
			case "Helicopter":
				return new Helicopter(idCounter++, p_name, p_coordinates);
			case "JetPlane":
				return new JetPlane(idCounter++, p_name, p_coordinates);
			default:
				throw new InvalidAircraftTypeException(
						"Unknown aircraft type: " + p_type + ". Supported types: Balloon, Helicopter, JetPlane");
		}
	}

}
