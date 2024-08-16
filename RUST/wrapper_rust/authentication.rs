pub fn auth() -> String {
    let mut user: String;
    loop {
	let mut prompt = "\nIndique el usuario:";
	user = get_user_input(prompt);
	if user.trim().to_lowercase() == "s" {
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    println!("\nSaliendo del sistema.");
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    std::process::exit(0);
	}
	prompt = "\nIndique la clave: ";
	let password = rpassword::prompt_password(prompt).unwrap();
	if password.trim().to_lowercase() == "s" {
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    println!("\nSaliendo del sistema.");
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    std::process::exit(0);
	}
	match validate_user(user.clone(), password) {
	    true => {
            println!("\nUsuario validado.");
            break;
	    },
	    false => println!("\nHay un error con el usuario o con la clave, por favor indique los datos nuevamente, o introduzca \"s\" para salir."),
	}
    }

    user
}

pub fn get_user_input(prompt: &str) -> String {
    let mut input = String::new();
    loop {
        println!("{} ", prompt);
        match std::io::stdin().read_line(&mut input) {
            Ok(_) => break,
            Err(error) => println!("Error procesando la información: {}", error),
        }
    }

    input.trim().to_string()
}

pub fn validate_user(user: String, password: String) -> bool {
    let users_file = std::fs::read_to_string("users.txt").unwrap();
    let users_file_lines: Vec<String> = users_file
        .split("\n")
        .map(|it| it.to_string())
        .collect();
    for line in users_file_lines {
        let parts: Vec<String> = line
            .split(";")
            .map(|it| it.to_string())
            .collect();
        if user == parts[0] && password == parts[1] {
            return true;
        }
    }

    false
}
