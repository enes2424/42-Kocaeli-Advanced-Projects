package com.enesates.avaj.simulator;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import com.enesates.avaj.simulator.exceptions.SimulationWriterException;

public class SimulationWriter {
	private static SimulationWriter instance = null;
	private PrintWriter writer;

	private SimulationWriter() {
		try {
			FileWriter fileWriter = new FileWriter("simulation.txt");
			writer = new PrintWriter(fileWriter);
		} catch (IOException e) {
			throw new SimulationWriterException("Failed to create simulation.txt file", e);
		}
	}

	public static SimulationWriter getInstance() {
		if (instance == null)
			instance = new SimulationWriter();

		return instance;
	}

	public void println(String message) {
		writer.println(message);
		writer.flush();
	}

	public void close() {
		if (writer != null) {
			writer.close();
		}
	}
}
