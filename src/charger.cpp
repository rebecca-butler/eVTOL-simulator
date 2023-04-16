class Charger {
public:
    Charger(int number_chargers_) : number_chargers(number_chargers_) {}

    set_number_aircraft_charging() {
        number_aircraft_charging++;
    }

    bool charger_available() {
        return number_aircraft_charging < number_chargers;
    }

private:
    int number_chargers;
    int number_aircraft_charging;
}
