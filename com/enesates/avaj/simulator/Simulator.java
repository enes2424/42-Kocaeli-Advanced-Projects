package com.enesates.avaj.simulator;

import com.enesates.avaj.simulator.exceptions.*;

import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.Files;
import java.io.IOException;
import java.util.List;

public class Simulator {
	public static void main(String[] args) {
		if (args.length != 1) {
			System.out.println("Usage: java com.enesates.avaj.Simulator <scenario file>");
			System.exit(1);
		}

		String scenarioFile = args[0];
		try {
			Path path = Paths.get(scenarioFile);
			List<String> lines = Files.readAllLines(path);
			int numberOfSimulations = Integer.parseInt(lines.get(0).trim());

			if (numberOfSimulations < 1)
				throw new InvalidScenarioException(
						"Number of simulations must be a positive integer: " + numberOfSimulations);

			lines.remove(0);

			WeatherTower weatherTower = new WeatherTower();
			AircraftFactory aircraftFactory = AircraftFactory.getInstance();

			for (String line : lines) {
				String[] parts = line.split(" ");

				if (parts.length != 5)
					throw new InvalidScenarioException(
							"Invalid aircraft description format. Expected: TYPE NAME LONGITUDE LATITUDE HEIGHT, got: "
									+ line);

				String type = parts[0];
				String name = parts[1];
				int longitude = Integer.parseInt(parts[2]);
				int latitude = Integer.parseInt(parts[3]);
				int height = Integer.parseInt(parts[4]);

				try {
					Coordinates coordinates = new Coordinates(longitude, latitude, height);
					Flyable aircraft = aircraftFactory.newAircraft(type, name, coordinates);
					aircraft.registerTower(weatherTower);
				} catch (InvalidCoordinatesException e) {
					throw new InvalidScenarioException(
							"Invalid coordinates for aircraft " + name + ": " + e.getMessage());
				} catch (InvalidAircraftTypeException e) {
					throw new InvalidScenarioException("Invalid aircraft type for " + name + ": " + e.getMessage());
				}
			}

			for (int i = 0; i < numberOfSimulations; i++) {
				weatherTower.changeWeather();
			}

			SimulationWriter.getInstance().close();

		} catch (IOException e) {
			System.out.println("Error reading scenario file: " + e.getMessage());
			System.exit(1);
		} catch (NumberFormatException e) {
			System.out.println("Error parsing number in scenario file: " + e.getMessage());
			System.exit(1);
		} catch (InvalidScenarioException e) {
			System.out.println("Invalid scenario: " + e.getMessage());
			System.exit(1);
		} catch (SimulationWriterException e) {
			System.out.println("Simulation writer error: " + e.getMessage());
			System.exit(1);
		} catch (Exception e) {
			System.out.println("Unexpected error: " + e.getMessage());
			System.exit(1);
		}
	}
}
