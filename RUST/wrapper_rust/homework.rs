pub fn homework(session_file: String, session_backup: bool, password: &str, user: String) -> u8 {
    let mut path = format!("./users/{}/homeworks", user);
    let files: Vec<std::ffi::OsString> = std::fs::read_dir(path)
	.unwrap()
	.map(|item| item.unwrap().file_name())
	.collect();
    let len = files.len();
    let mut prompt: String;
    if len == 0 {
	prompt = "\nEl directorio de tareas está vacío.".to_string();
	crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	std::thread::sleep(std::time::Duration::new(1, 0));
	return 1;
    }
    prompt = "\nPor favor selecciona una opción de la lista:\n".to_string();
    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
    std::thread::sleep(std::time::Duration::new(1, 0));
    for (i, file) in files.iter().enumerate() {
	let file = &file.clone().into_vec().iter().map(|item| *item as char).collect::<String>();
	prompt =
	    if i < 9 {
		format!("  {}. {}", i+1, &file[..file.len()-4])
	    } else {
		format!(" {}. {}", i+1, &file[..file.len()-4])
	    };
	crate::session::print_and_save(prompt, session_file.clone(), session_backup);
    }
    prompt = "\nOpción:".to_string();
    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
    let mut user_input: usize;
    loop {
	user_input = crate::get_user_input(session_file.clone(), session_backup);
	if user_input >= 1 && user_input <= len {
	    prompt = format!("\nSe muestran los ejercicios de la lista de tareas seleccionada:\n");
	    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    break;
	} else {
	    prompt = "\nOpción no válida.\n\nIndique la opción nuevamente:\n".to_string();
	    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	    continue;
	}
    }

    use std::os::unix::ffi::OsStringExt;
    let partial_path = &files[user_input-1].clone().into_vec().iter().map(|item| *item as char).collect::<String>();
    path = format!("./users/{}/homeworks/{}", user, partial_path);
    let exercises_file = std::fs::read_to_string(path.clone()).unwrap();
    let exercises: Vec<&str> = exercises_file
	.split("\n")
	.filter(|item| !item.is_empty())
	.collect::<Vec<&str>>();
    let operations = std::collections::HashMap::from([
	("1", "Suma"),
	("2", "Resta"),
	("3", "Multiplicación"),
	("4", "División"),
	("5", "Factores primos"),
	("6", "Mínimo Común Múltiplo"),
	("7", "Máximo Común Divisor"),
	("8", "Operaciones combinadas"),
	("9", "Operaciones con fracciones"),
	("10", "Suma con decimales"),
	("11", "Resta con decimales"),
	("12", "Multiplicación con decimales"),
	("13", "División con decimales"),
    ]);
    for (i, item) in exercises.iter().enumerate() {
	let tmp = item.split(" ").collect::<Vec<&str>>();
	prompt =
	    if i < 9 {
		format!("  {}. {}: {} - {}", i+1, operations[tmp[0]], tmp[1], tmp[2])
	    } else {
		format!(" {}. {}: {} - {}", i+1, operations[tmp[0]], tmp[1], tmp[2])
	    };
	println!("{prompt}");
	if session_backup {
	    crate::session::append_to_file(session_file.clone(), prompt.clone());
	}
    }
    let (_descriptions, programs) = crate::get_options();
    std::thread::sleep(std::time::Duration::new(1, 0));
    prompt = "\nVamos a resolver los ejercicios pendientes. En cualquier momento puedes introducir la letra \"s\" si no deseas terminar el ejercicio.".to_string();
    println!("{prompt}");
    if session_backup {
	crate::session::append_to_file(session_file.clone(), prompt.clone());
    }
    for i in 0..exercises.len() {
	let option = exercises[i].split(" ").collect::<Vec<&str>>();
	let (option1, option2, option3) = (option[0].parse::<usize>().unwrap(), option[1], option[2]);
	if option3 == "Resuelto" { continue; }
	prompt = format!("\n  Siguiente ejercicio -> {} -> {}", operations[option[0]], option2);
	std::thread::sleep(std::time::Duration::new(1, 0));
	crate::session::print_and_save(prompt.clone(), session_file.clone(), session_backup);
	match option1 {
	    1..=13 => {
		let exit_code = crate::launch_homework_program(option1, programs.clone(), session_file.clone(), session_backup, password, option2);
		if exit_code == 0 {
		    mark_as_solved(i, path.clone());
		}
	    },	
	    _ => (),
	}
	std::thread::sleep(std::time::Duration::new(1, 0));

	prompt = "\nIntroduce \"s\" para salir del sistema, o presiona ENTER para continuar con el siguiente ejercicio.\n\nOpción:".to_string();
	crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	crate::get_user_input(session_file.clone(), session_backup);
//	    continue;
    }

    return 0;
}

pub fn mark_as_solved(index: usize, homeworks_file: String) {
    crate::session::create_file("tmp_file.txt".to_string());
    use std::fs::read_to_string;
    let file = read_to_string(homeworks_file.clone()).unwrap();
    let exercises: Vec<&str> = file
	.split("\n")
	.collect();
    for i in 0..exercises.len() {
	if i == index {
	    let parts: Vec<&str> = exercises[i].split(" ").collect();
	    let line = format!("{} {} {}", parts[0], parts[1], "Resuelto");
	    crate::session::append_to_file("tmp_file.txt".to_string(), line);
	    continue;
	}
	crate::session::append_to_file("tmp_file.txt".to_string(), exercises[i].to_string());
    }

    std::fs::rename("tmp_file.txt", homeworks_file).unwrap();
}
