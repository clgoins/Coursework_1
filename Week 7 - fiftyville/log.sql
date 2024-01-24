-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Started with .schema to get an idea of what kind of information the db holds
.schema

-- Took a look at the crime_scene_reports table to get an idea of the information contained
SELECT * FROM crime_scene_reports;

-- Narrowed crime_scene_reports down to July 28th 2021
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28;
--Theft took place at 10:15am at bakery. Three witnesses.

--Took a look at interviews from the crime scene from that day
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;
-- thief got into car and left bakery within 10 minutes of theft - from Ruth
-- thief used ATM on Leggett Street early in morning - from Eugene
-- thief made brief phone call as they were leaving bakery, planning to take flight out of Fiftyville next day. Likely talking to accomplice. - from Raymond

-- Took a look at bakery security logs
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28;

-- Took a look at ATM transactions made on Leggett Street the day of the crime.
SELECT * FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street';

-- Took a look at phone records from the day the theft occurred.
SELECT * FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28;

-- Made a list of names of people that were leaving the bakery within 10 minutes of the crime and that made a short phone call the day of the crime.
SELECT name
  FROM people
  WHERE phone_number IN
        (SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60)
  AND license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 5 AND 25 AND activity = 'exit');

-- Sophia, Diana, Kelsey, and Bruce are all potential suspects. Let's see if any of them made an ATM transaction on Leggett Street the day of the crime.
SELECT name
FROM people
WHERE id IN
    (SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
        (SELECT account_number
        FROM atm_transactions
        WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street'))
AND name IN
    (SELECT name
    FROM people
    WHERE phone_number IN
        (SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 70)
    AND license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 5 AND 25 AND activity = 'exit'));


-- This narrows it down to Diana and Bruce.
-- I want to see if either of them were passengers on a flight the day after the theft

SELECT name
FROM people
WHERE passport_number IN
    (SELECT passport_number
    FROM passengers
    WHERE flight_id IN
        (SELECT id
        FROM flights
        WHERE year = 2021 AND MONTH = 7 AND DAY = 29))
AND (name = 'Bruce' OR name = 'Diana');

-- Appears as though they were both on a plane that day. Let's see if it was the same flight.

SELECT name, flight_id
FROM people JOIN passengers
    ON people.passport_number = passengers.passport_number
WHERE name = 'Bruce' OR name = 'Diana' AND flight_id IN
    (SELECT id
    FROM flights
    WHERE year = 2021 AND month = 7 AND day = 29);


-- Seems they were not on the same flight. Diana was on flight 18, and Bruce on flight 36.
-- Raymond said during his interview that the thief wanted the earliest flight out of Fiftyville on the 29th. Let's look at the flight schedule for the day.

SELECT city, flights.id, hour, minute
FROM airports JOIN flights ON airports.id = flights.origin_airport_id
WHERE year = 2021 AND month = 7 AND day = 29;

-- Flight 36 was the earliest flight from fiftyville on the 29th, and Bruce had a ticket for that flight.
-- Let's check where flight 36 was heading
SELECT city
FROM airports
WHERE id IN
    (SELECT destination_airport_id
    FROM flights
    WHERE id = 36);

-- Flight 36 was headed to New York City
-- Finally let's check who Bruce was calling the day of the theft
SELECT name
FROM people
WHERE phone_number IN
    (SELECT receiver
    FROM phone_calls
    WHERE caller =
        (SELECT phone_number
        FROM people
        WHERE name = 'Bruce') AND year = 2021 AND month = 7 AND day = 28 AND duration < 60);

-- Looks like Bruce called Robin
-- Bruce stole the CS50 duck
-- Robin bought the plane ticket
-- Bruce escaped to New York City