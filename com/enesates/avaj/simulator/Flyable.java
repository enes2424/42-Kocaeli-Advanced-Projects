package com.enesates.avaj.simulator;

public abstract class Flyable {

	protected WeatherTower weatherTower;

	public abstract void updateConditions();

	public void registerTower(WeatherTower p_tower) {
		this.weatherTower = p_tower;
		p_tower.register(this);
	}

}
