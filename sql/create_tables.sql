create table user (
    discord_id INT primary key,
    moodle_username char(255) not NULL,
    moodle_password char(255) NOT NULL
);

create table module (
    module_id INT PRIMARY KEY,
    module_name CHAR(255) not NULL
);

create table quiz (
    quiz_id INT primary key,
    quiz_name CHAR(255) NOT NULL,
    deadline INT NOY NULL,
    module_id INT not NULL REFERENCES module(module_id),
    max_marks INT
);

create table quiz_result (
    quiz_id INT REFERENCES quiz(quiz_id) NOT NULL,
    discord_id INT REFERENCES user(discord_id) NOT NULL,
    marks INT NOT NULL,
    grade VARCHAR(5) NOT NULL
);

