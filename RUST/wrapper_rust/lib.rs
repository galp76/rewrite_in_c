use std::fs::read_to_string;

pub fn get_options() -> (Vec<String>, Vec<String>) {
    let mut descriptions: Vec<String> = Vec::new();
    let mut programs: Vec<String> = Vec::new();
    for item in read_to_string("options.txt").unwrap().lines() {
        let tmp: Vec<&str> = item.split(';').collect();
        descriptions.push(tmp[0].to_string());
        programs.push(tmp[1].to_string());
    }

    (descriptions, programs)
}

pub fn display_options(options: &Vec<String>, display_prompt: bool, session: String, session_backup: bool) {
    if display_prompt {
	let prompt = "\nEn cualquier momento puedes introducir la letra \"s\" si no deseas terminar el ejercicio.\n\nPor favor indica una de las siguientes opciones y presiona ENTER:\n";
	println!("{prompt}");
	if session_backup {
	    session::append_to_file(session.clone(), prompt.to_string());
	}
    }
    for (i, item) in options.iter().enumerate() {
	let prompt =
	    if i < 9 {
		format!("  {}. {}", i+1, item)
	    } else {
		format!(" {}. {}", i+1, item)
	    };
        println!("{prompt}");
	if session_backup {
	    session::append_to_file(session.clone(), prompt);
	}
    } 
    println!("\nOpción:");
    if session_backup {
	session::append_to_file(session, "\nOpción:".to_string());
    }
}

pub fn get_user_input(session2: String, session_backup: bool) -> usize {
    loop {
        let mut option = String::new();
        std::io::stdin().read_line(&mut option).expect("Unable to read from stdin");
        if option.trim().to_lowercase().as_str() == "s" {
            if session_backup {
                session::append_to_file(session2.clone(), option.clone());
            }
            std::thread::sleep(std::time::Duration::new(1, 0));
            println!("\nSaliendo del sistema.\n");
            if session_backup {
                session::append_to_file(session2, "\nSaliendo del sistema.\n".to_string());
            }
            std::thread::sleep(std::time::Duration::new(1, 0));
            std::process::exit(0);
        }
        match option.trim().parse::<usize>() {
            Ok(input) => {
                if session_backup {
                    session::append_to_file(session2, input.to_string());
                }
                return input;
	        },
            Err(_) => {
                if session_backup {
                    session::append_to_file(session2, option);
                }
                return 0;
            },
        }
    }       
}

pub fn launch_program(input: usize, programs: Vec<String>, session: String, session_backup: bool, password: &str) {
//    std::thread::sleep(std::time::Duration::new(1, 0));
    match input {
	1..=14 => {
	    let mut program = std::process::Command::new(&programs[input-1])
		.arg(session)
		.arg(session_backup.to_string())
		.arg(password)
		.spawn()
		.expect("Unable to launch program");
	    
            program.wait().expect("Failed to wait for child program");
	},
        _ => todo!(),
    }
}

pub fn launch_homework_program(input: usize, programs: Vec<String>, session: String, session_backup: bool, password: &str, operation: &str) -> u8 {
//    std::thread::sleep(std::time::Duration::new(1, 0));
    match input {
	1..=13 => {
	    let mut program = std::process::Command::new(&programs[input-1])
		.arg(session)
		.arg(session_backup.to_string())
		.arg(password)
		.arg(operation)
		.spawn()
		.expect("Unable to launch program");
	    
	    match program.wait() {
		Ok(exit_status) =>
		    if exit_status.code() == Some(1) {
			std::thread::sleep(std::time::Duration::new(1, 0));
			println!("Saliste antes de finalizar, el ejercicio sigue pendiente.");
			return 1;
		    } else {
			std::thread::sleep(std::time::Duration::new(1, 0));
			println!("\nEl ejercicio ha sido marcado como resuelto.");
			return 0;
		    },
		Err(_) => return 2,
	    }
	},
        _ => todo!(),
    }
}

pub mod session {
    use std::fs;
    use std::io::Write;

    pub fn append_to_file(filename: String, data: String) {
	let mut file = fs::OpenOptions::new()
	    .write(true)
	    .append(true)
	    .open(filename)
	    .unwrap();
	write!(file, "{}\n", &data).expect("Unable to write data");
    }

    pub fn create_file(filename: String) {
	fs::File::create(filename.clone())
	    .expect(format!("Unable to create file {}.", filename).as_str());
    }

    pub fn print_and_save(prompt: String, session_file: String, session_backup: bool) {
	println!("{prompt}");
	if session_backup {
	    crate::session::append_to_file(session_file, prompt.to_string());
	}
    }
}

pub mod homework;
pub mod practice;
pub mod authentication;
pub mod admin;
