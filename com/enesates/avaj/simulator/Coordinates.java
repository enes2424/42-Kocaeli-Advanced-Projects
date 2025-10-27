package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.InvalidCoordinatesException;

public class Coordinates {
	private int longitude;
	private int latitude;
	private int height;

	Coordinates(int p_longitude, int p_latitude, int p_height) throws InvalidCoordinatesException {
		if (p_longitude < 0 || p_latitude < 0 || p_height < 0)
			throw new InvalidCoordinatesException("Coordinates cannot be negative: longitude=" + p_longitude
					+ ", latitude=" + p_latitude + ", height=" + p_height);

		if (p_longitude == 0 || p_latitude == 0)
			throw new InvalidCoordinatesException(
					"Longitude and Latitude cannot be zero: longitude=" + p_longitude + ", latitude=" + p_latitude);

		if (p_height > 100)
			throw new InvalidCoordinatesException("Height cannot be greater than 100: " + p_height);

		if (p_latitude > 90)
			throw new InvalidCoordinatesException("Latitude must be between 1 and 90: " + p_latitude);

		this.longitude = p_longitude % 360;

		if (this.longitude == 0)
			this.longitude = 360;

		this.latitude = p_latitude;
		this.height = p_height;
	}

	public int getLongitude() {
		return longitude;
	}

	public int getLatitude() {
		return latitude;
	}

	public int getHeight() {
		return height;
	}
}
