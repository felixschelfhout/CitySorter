# Building a Binary Search Tree from API Data

Write a C program that performs the following:
1. Fetches data from a public API:
  - Use the following endpoint to retrieve the list of cities in San Marino
    ```
    https://countriesnow.space/api/v0.1/countries/cities
    {
    "country": "San Marino"
    }
    ```
    API documentation: Get all cities in a specified country

    request
    ```
    curl --location 'https://countriesnow.space/api/v0.1/countries/cities' \
    --data '{
        "country": "nigeria"
    }'
    ```
    response
    ```
    {
    "error": false,
    "msg": "cities in nigeria retrieved",
    "data": [
        "Aba",
        "Abakaliki",
        "Abeokuta",
        "Abraka",
        "Abraka",
        "Abuja",
        "Ado-Ekiti",
        "Adodo",
        "Aganga",
        "Agege",
        "Agidingbi",
        "Ajegunle",
        "Ajuwon",
        "Akure",
        "Alimosho",
        "Anambra",
        "Apapa",
        "Ayobo",
        "Benin City",
        "Birnin Kebbi",
        "Bonny",
        "Burutu",
        "Bwari",
        "Calabar",
        "Chafe",
        "Damaturu",
        "Egbeda",
        "Ekpoma",
        "Enugu",
        "Forum",
        "Funtua",
        "Ibadan",
        "Ido",
        "Ifako",
        "Igando",
        "Igueben",
        "Ikeja",
        "Ikorodu",
        "Ikotun",
        "Ile-Ife",
        "Ilesa",
        "Ilorin",
        "Ipaja",
        "Iseri-Oke",
        "Isolo",
        "Jalingo",
        "Jos",
        "Kaduna",
        "Kano",
        "Kebbi",
        "Lagos",
        "Lekki",
        "Lokoja",
        "Magodo",
        "Makurdi",
        "Maryland",
        "Minna",
        "Mogho",
        "Mowe",
        "Mushin",
        "Nsukka",
        "Obafemi",
        "Obudu",
        "Odau",
        "Ojo",
        "Ojota",
        "Ondo",
        "Onigbongbo",
        "Orile Oshodi",
        "Oshodi",
        "Osogbo",
        "Ota",
        "Owerri",
        "Oworonsoki",
        "Port Harcourt",
        "Shomolu",
        "Suleja",
        "Suru-Lere",
        "Tara",
        "Ughelli",
        "Ungwan Madaki",
        "Uyo",
        "Warri",
        "Warri",
        "Yaba",
        "Yola",
        "Zaria"
    ]
    }
    ```
2. Parses the JSON response to extract the list of cities of San Marino

3. Builds a Binary Search Tree (BST) using the extracted city names.
  - Each city name should be inserted into the BST in the correct order.

4. Implements user commands using a simple state machine, supporting the following inputs:
    - `print` Display the BST in a rotated format (right -> root -> left).
    - `add [city]` Add a new city to the BST.
    - `remove [city]` Remove a city from the BST.
    - `stop` Exit the program

5. Handle kill signals gracefully, ensuring that any necessary cleanup is performed before the program exits.

# Expected Deliverables
Your submission should include:
1. All project files (source code and any headers).
2. Build instructions (e.g. Makefile or clear compilation commands).
3. A short explanation describing: 
   The libraries used (e.g. libcurl for HTTP requests, cJSON or Any challenges or issues you encountered while completing the task.)

If certain parts are not fully functional, provide:
- A brief explanation of what doesnʼt work.
- A clear plan or approach for how you would fix or complete it.

# Evaluation Criteria
- Clarity of implementation and structure.
- Understanding of data structures (especially BSTs).
- Proper API handling and JSON parsing.
- Good documentation and reasoning about design decisions.
- Thoughtful problem-solving, even if your code isnʼt completely finished.