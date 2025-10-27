package com.enesates.avaj.simulator;

import java.util.ArrayList;
import java.util.List;

public class Tower {
	private List<Flyable> observers = new ArrayList<>();

	public void register(Flyable p_flyable) {
		if (p_flyable != null && !observers.contains(p_flyable)) {
			observers.add(p_flyable);

			if (p_flyable instanceof Aircraft aircraft) {
				String message = "Tower says: " + aircraft.getIdentifier() + " registered to weather tower.";
				SimulationWriter.getInstance().println(message);
			}
		}
	}

	public void unregister(Flyable p_flyable) {
		observers.remove(p_flyable);

		if (p_flyable instanceof Aircraft aircraft) {
			String message = "Tower says: " + aircraft.getIdentifier() + " unregistered from weather tower.";
			SimulationWriter.getInstance().println(message);
		}
	}

	protected void conditionChanged() {
		for (int i = 0; i < observers.size();) {
			Flyable flyable = observers.get(i);
			flyable.updateConditions();

			if (flyable instanceof Aircraft aircraft && aircraft.hasLanded()) {
				String message = aircraft.getIdentifier() + " landing.";
				SimulationWriter.getInstance().println(message);
				unregister(flyable);
			} else {
				i++;
			}
		}
	}

	protected void logObservers() {
		for (Flyable flyable : observers)
			if (flyable instanceof Aircraft aircraft)
				aircraft.log();
	}
}
