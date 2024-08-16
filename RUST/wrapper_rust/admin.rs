pub fn init() {
    loop {
	let prompt = "\nIntroduce \"s\" para salir del sistema, o una de las siguientes opciones:\n\n  Usuarios:\n    1. Crear usuario.\n    2. Eliminar usuario.\n    3. Mostrar lista de usuarios.\n\n  Tareas:\n    4. Crear una lista de tareas.\n    5. Mostrar una lista de tareas.\n    6. Eliminar una lista de tareas.\n    7. Editar una lista de tareas.\n    8. Copiar una lista de tareas entre dos usuarios.\n\n  Grupos:\n    9. Mostrar la lista de los grupos creados.\n   10. Crear un nuevo grupo.\n   11. Eliminar un grupo.\n   12. Asignar o cambiar el grupo al que pertenece un usuario.\n   13. Mostrar los usuarios en un grupo.\n   14. Asignar una lista de tareas a todos los usuarios de un grupo.\n\nOpción:";
	let mut option = String::new();
	println!("{}", prompt);
        match std::io::stdin().read_line(&mut option) {
	    Ok(_) => (),
	    Err(error) => {
		println!("\nHubo un error leyendo la opción seleccionada: {}.", error);
		continue;
	    }
	}
	if option.trim().to_lowercase() == "s" {
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    println!("\nSaliendo del sistema.\n");
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    std::process::exit(0);
	}
	std::thread::sleep(std::time::Duration::new(1, 0));
        match option.trim().parse::<u8>() {
	    Ok(1) => users::create_user(),
	    Ok(2) => users::delete_user(),
	    Ok(3) => users::show_users(),
	    Ok(4) => files::create_homework_file(),
	    Ok(5) => files::show_homework_file_content(),
	    Ok(6) => files::delete_homework_file(),
	    Ok(7) => files::edit_homework_file(),
	    Ok(8) => files::duplicate_homework_file(),
	    Ok(9) => groups::list_groups(),
	    Ok(10) => groups::create_group(),
	    Ok(11) => groups::delete_group(),
	    Ok(12) => groups::include_user_in_group(),
	    Ok(13) => groups::list_users_in_a_group(),
	    Ok(14) => groups::assign_homework_to_group(),
	    Ok(x) => {
		println!("\nLa opción {} no es válida.\n", x);
		continue;
	    },	
	    Err(error) => {
		println!("Hubo un error en la operación: {}", error);
		continue;
	    },
        }
	std::thread::sleep(std::time::Duration::new(1, 0));
    }           

}

pub mod users {
    // retorna true si el usuario es nuevo
    pub fn validate_new_user(new_user: String) -> bool {
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
	    if new_user == parts[0] {
		return false
	    }
	}

	true
    }

    pub fn delete_empty_lines(filename: String) {
	let file = std::fs::read_to_string(filename.clone()).unwrap();
	let file_lines: Vec<String> = file
	    .split("\n")
	    .map(|it| it.to_string())
	    .collect();
	crate::session::create_file("tmp_file.txt".to_string());    
	for line in file_lines {
	    if line.trim() != "" {
		crate::session::append_to_file("tmp_file.txt".to_string(), line);
	    }
	}

	std::fs::rename("tmp_file.txt", filename.as_str()).unwrap();
    }

    pub fn create_user() {
	loop {
	    let mut prompt = "\nPor favor indique el nuevo usuario:";
	    println!("{}", prompt);
	    let mut new_user = String::new();
	    match std::io::stdin().read_line(&mut new_user) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    new_user = new_user.trim().to_string();
	    if !validate_new_user(new_user.clone()) {
		prompt = "\nEl usuario indicado ya existe.";
		println!("{}", prompt);
		continue;
	    }

	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nIndique la clave:";
	    println!("{}", prompt);
	    let mut password = String::new();
	    match std::io::stdin().read_line(&mut password) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo la clave: {}", error);
		    continue;
		},
	    }
	    password = password.trim().to_string();

	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nIndica el grupo al que pertenece el usuario:\n";
	    println!("{}", prompt);
	    let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	    let groups_lines: Vec<&str> = groups_file
		.split("\n")
		.filter(|it| !it.is_empty())
		.collect();
	    for (i, group) in groups_lines.iter().enumerate() {
		println!("{}", format!("  {}. {}", i+1, group));
	    }
	    prompt = "\nOpción:";
	    println!("{}", prompt);
	    let mut group = String::new();
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el grupo: {}", error);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    let number: usize;
	    match group.parse::<usize>() {
		Ok(w) if w <= groups_lines.len() => number = w,
		Ok(_) => {
		    println!("\nLa opción indicada no está entre las opciones.");
		    continue;
		},
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}", error);
		    continue;
		},
	    }	
		
	    let new_user_data = format!("{};{};{}", new_user, password, groups_lines[number - 1]);
	    crate::session::append_to_file("users.txt".to_string(), new_user_data);
	    std::fs::create_dir_all(format!("users/{}/homeworks", new_user)).unwrap();
	    std::fs::create_dir_all(format!("users/{}/sessions", new_user)).unwrap();
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    println!("\nUsuario {} creado.", new_user);
	    break;
	}
    }

    pub fn delete_user() {
	loop {
	    println!("\nPor favor indique el usuario a eliminar: ");
	    let mut new_user = String::new();
	    match std::io::stdin().read_line(&mut new_user) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    new_user = new_user.trim().to_string();
	    if validate_new_user(new_user.clone()) {
		println!("\nEl usuario indicado no existe.");
		continue;
	    }

	    let users_file = std::fs::read_to_string("users.txt").unwrap();
	    let users_file_lines: Vec<String> = users_file
		.split("\n")
		.map(|it| it.to_string())
		.collect();
	    crate::session::create_file("tmp_users.txt".to_string());
	    for line in users_file_lines {
		let parts: Vec<String> = line
		    .split(";")
		    .map(|it| it.to_string())
		    .collect();
		if parts[0] != new_user {
		    crate::session::append_to_file("tmp_users.txt".to_string(), line);
		}
	    }
	    std::fs::rename("tmp_users.txt", "users.txt").unwrap();
	    delete_empty_lines("users.txt".to_string());
	    let dir_to_delete = format!("users/{}", new_user);
	    std::fs::remove_dir_all(dir_to_delete).unwrap();
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    println!("\nUsuario {} eliminado.", new_user);
	    break;
	}
    }

    pub fn show_users() {
	let users_file = std::fs::read_to_string("users.txt").unwrap();
	let users_file_lines: Vec<String> = users_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .map(|it| it.to_string())
	    .collect();
	println!("");  // para poner una línea en blanco antes de imprimir los usuarios
	for line in users_file_lines {
	    let parts: Vec<String> = line
		.split(";")
		.map(|it| it.to_string())
		.collect();
	    println!("Usuario: {}\t\tClave: {}", parts[0], parts[1]);
	}
    }
}

pub mod files {
    // los dos "bool" son para cuando se usa para crear archivos de tareas nuevos
    pub fn append_exercise_to_homework_file(path: String, one_time: bool) -> bool {
	let new_line: String;
	let mut prompt: &str;
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

	loop {
	    let mut option = String::new();
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    if one_time {
		prompt = "\nIntroduce alguna de las siguientes opciones, para indicar el tipo de operación:\n";
		println!("{}", prompt);
	    } else {
		prompt = "\nIntroduce \"s\" para terminar, o alguna de las siguientes opciones, para indicar el tipo de operación:\n";
		println!("{}", prompt);
	    }
	    for i in 0..operations.len() {
		println!("  {}. {}", i+1, operations[(i+1).to_string().as_str()]);
	    }
	    println!("\nOpción:");	    
	    match std::io::stdin().read_line(&mut option) {
		Ok(_) => (),
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}.", error);
		    continue;
		},
	    }
	    if !one_time && option.trim().to_lowercase() == "s" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nArchivo de tareas creado.\n");
		std::thread::sleep(std::time::Duration::new(1, 0));
		return false;
	    }
	    option = option.trim().to_string();

	    let mut operation = String::new();
	    prompt = "\nAhora por favor indica la operación:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut operation) {
		Ok(_) => (),
		Err(error) => {
		    println!("Hubo un error leyendo la operación: {}.", error);
		    continue;
		},
	    }
	    operation = operation.trim().to_string();
	    new_line = format!("{} {} Pendiente", option, operation);
	    break;
	}
	add_line(path.clone(), new_line.as_str());
	return true;
    }    

    pub fn create_homework_file() {
	let mut filename = String::new();
	let mut prompt: String;
	loop {
	    let mut user = String::new();
	    prompt = String::from("\nPor favor indique el usuario, o introduzca \"s\" para salir:");
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut user) {
		Ok(_) => (),
		Err(error) => {
		    println!("Hubo un error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    if user.trim().to_lowercase() == "s" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nSe canceló la creación del archivo de tareas. Saliendo del sistema.\n");
		std::thread::sleep(std::time::Duration::new(1, 0));
		std::process::exit(0);
	    }
	    if user.trim() == "admin" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nNo se pueden crear lista de tareas para este usuario.");
		std::thread::sleep(std::time::Duration::new(1, 0));
		continue;
	    }
	    user = user.trim().to_string();
	    if crate::admin::users::validate_new_user(user.clone()) {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nEl usuario no está registado.");
		std::thread::sleep(std::time::Duration::new(1, 0));
		continue;
	    }
	    prompt = "\nAhora por favor indique el nombre del archivo:".to_string();
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut filename) {
		Ok(_) => (),
		Err(error) => {
		    println!("Hubo un error leyendo el nombre del archivo: {}", error);
		    continue;
		},
	    }
	    filename = format!("users/{}/homeworks/{}.txt", user, filename.trim());
	    crate::session::create_file(filename.clone());
	    break;
	}

	let mut must_continue = true;
	while must_continue {
	    must_continue = append_exercise_to_homework_file(filename.clone(), false);
	}
    }

    pub fn get_user_and_filename() -> String {
	let mut prompt: String;
	let mut user = String::new();
	loop {
	    prompt = String::from("\nPor favor introduce \"s\" para salir, o indica el nombre del usuario:");
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut user) {
		Ok(_) => (),
		Err(error) => {
		    std::thread::sleep(std::time::Duration::new(1, 0));
		    println!("Hubo un error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    if user.trim().to_lowercase() == "s" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nSaliendo del sistema.\n");
		std::thread::sleep(std::time::Duration::new(1, 0));
		std::process::exit(0);
	    }
	    user = user.trim().to_string();
	    break;
	}
	let mut path = format!("users/{user}/homeworks");
	use std::os::unix::ffi::OsStringExt;
	let files: Vec<std::ffi::OsString> = std::fs::read_dir(path.clone())
	    .unwrap()
	    .map(|item| item.unwrap().file_name())
	    .collect();
	std::thread::sleep(std::time::Duration::new(1, 0));
	loop {
	    let mut option = String::new();
	    prompt = "\nIntroduce \"s\" para salir, o alguna de las siguientes opciones:\n".to_string();
	    println!("{}", prompt);	
	    for (i, file) in files.iter().enumerate() {
		let file = &file.clone().into_vec().iter().map(|item| *item as char).collect::<String>();
		prompt = format!("  {}. {}", i+1, &file[..file.len()-4]);
		println!("{}", prompt);
	    }
	    println!("\nOpción:");
	    match std::io::stdin().read_line(&mut option) {
		Ok(_) => (),
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}.", error);
		    continue;
		},
	    }
	    if option.trim().to_lowercase() == "s" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nSaliendo del sistema.\n");
		std::thread::sleep(std::time::Duration::new(1, 0));
		std::process::exit(0);
	    }
	    let user_input: usize;
	    match option.trim().parse::<usize>() {
		Ok(w) => user_input = w,
		Err(error) => {
		    println!("La opción indicada no es válida: {}", error);
		    continue;
		},
	    }
	    if user_input >= 1 && user_input <= files.len() {
		// establecemos el nuevo path para leer los ejercicios
		let file = &files[user_input-1].clone().into_vec().iter().map(|item| *item as char).collect::<String>();
		path = format!("{}/{}", path, file);
		break;
	    } else {
		std::thread::sleep(std::time::Duration::new(1, 0));
		prompt = "\nOpción no válida.\n\nIndique la opción nuevamente:\n".to_string();
		println!("{}", prompt);
		std::thread::sleep(std::time::Duration::new(1, 0));
		continue;
	    }
	}

	path
    }

    // devuelve la longitud del vector de ejercicios para usarlo en edit_homework_file
    pub fn list_exercises_in_a_file(path: String) -> usize {
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
	let exercises_file = std::fs::read_to_string(path).unwrap();
	let exercises_file_lines: Vec<&str> = exercises_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .collect();
	for (i, line) in exercises_file_lines.iter().enumerate() {
	    let parts: Vec<&str> = line
		.split(" ")
		.collect();
	    println!("  {}. {}: {} {}", i+1, operations[parts[0]], parts[1], parts[2]);
	}

	exercises_file_lines.len()
    }

    pub fn show_homework_file_content() {
	let path = get_user_and_filename();
	std::thread::sleep(std::time::Duration::new(1, 0));
	let prompt = format!("\nSe muestra el contenido de la lista de tareas seleccionada:\n");
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	list_exercises_in_a_file(path);
    }

    pub fn delete_homework_file() {
	loop {
	    let path = get_user_and_filename();
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    let prompt = format!("\nBorrando el archivo {}.\n", path);
	    println!("{}", prompt);
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    match std::fs::remove_file(path) {
		Ok(_) => {
		    println!("\nArchivo borrado.\n");
		    break;
		},
		Err(error) => {
		    println!("\nHubo en error borrando el archivo: {}", error);
		    continue;
		},
	    }
	}	    
    }

    // se usa en edit_homework_file
    fn add_line(path: String, line: &str) {
	use std::io::Write;    
	let mut file = std::fs::OpenOptions::new()
	    .append(true)
	    .open(path)
	    .unwrap();
	write!(file, "{}\n", line)
	    .expect("Unable to add task.");
    }

    // se usa en edit_homework_file
    fn delete_line(path: String, number: usize) {
	use std::io::Write;    
	let mut new_file = std::fs::OpenOptions::new()
	    .create(true)
	    .write(true)
	    .open("tmp.txt")
	    .unwrap();

	for (i, item) in std::fs::read_to_string(path.clone()).unwrap().lines().enumerate() {
	    if i != number {
		write!(new_file, "{}\n", item).expect("Unable to create tmp.txt");
	    }
	}

	std::fs::rename("tmp.txt", path).unwrap();
    }

    // se usa en edit_homework_file
    fn interchange_lines(path: String, actual_id: usize, new_id: usize) {
	if actual_id == new_id { return; }
	use std::io::Write;    
	let file = std::fs::read_to_string(path.clone()).unwrap();
	let tmp_vector: Vec<&str> = file.lines().collect();
	let mut new_positions: Vec<&str> = vec![];
	let task = tmp_vector[actual_id];
	for (i, item) in tmp_vector.iter().enumerate() {
	    if i == actual_id { continue; }
	    if i == new_id {
		if new_id < actual_id {
		    new_positions.push(task);
		    new_positions.push(item);
		} else {
		    new_positions.push(item);
		    new_positions.push(task);
		}
		continue;
	    }
	    new_positions.push(item);
	}

	let mut new_file = std::fs::OpenOptions::new()
	    .create(true)
	    .write(true)
	    .open("tmp.txt")
	    .unwrap();

	for item in new_positions {
	    write!(new_file, "{}\n", item).expect("Unable to create tmp.txt");
	}

	std::fs::rename("tmp.txt", path).unwrap();
    }

    pub fn edit_homework_file() {
	let mut option = String::new(); // para guardar la opción de edición: eliminar línea o insertar nueva línea
	let mut number = String::new(); // para guardar el número de la línea indicada por el usuario
	let line_number: usize; // "number" convertido en usize para usarlo borrando y acomodando la linea modificada en su lugar
	let mut length: usize; // cantidad de ejercicios en el archivo de tareas, para usarlo cuando se va a reubicar la nueva linea en su lugar
	let path = get_user_and_filename();
	let mut prompt: &str;
	loop {
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nPor favor indica una opción:\n\n  1. Eliminar un ejercicio.\n  2. Insertar un nuevo ejercicio.\n\nOpción:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut option) {
		Ok(_) => (),
		Err(error) => {
		    println!("\nHubo un error leyendo la opción de edición: {}.\n", error);
		    continue;
		},
	    }
	    option = option.trim().to_string();
	    if option == "1" || option == "2" {
		break;
	    } else {
		prompt = "\nLa opción indicada no es válida.\n";
		println!("{}", prompt);
		option.clear();
		continue;
	    }
	}

	std::thread::sleep(std::time::Duration::new(1, 0));
	if option == "1" {
	    prompt = "\nPor favor indica la posición del ejercicio a eliminar:\n";
	    println!("{}", prompt);
	} else {
	    prompt = "\nPor favor indica la posición en la que se va a insertar el ejercicio:\n";
	    println!("{}", prompt);
	}
	std::thread::sleep(std::time::Duration::new(1, 0));

	loop {
	    length = list_exercises_in_a_file(path.clone());
	    prompt = "\nPosición:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut number) {
		Ok(_) => (),
		Err(error) => {
		    println!("\nHubo un error leyendo el número del ejercicio: {}.\n", error);
		    continue;
		},
	    }
	    match number.trim().parse::<usize>() {
		Ok(w) => {
		    line_number = w;
		    break;
		},
		Err(error) => {
		    println!("\nHubo un error leyendo el número del ejercicio: {}.\n", error);
		    continue;
		}
	    }
	}

	if option == "1" {
	    delete_line(path.clone(), line_number-1);
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nEjercicio eliminado.\n";
	    println!("{}", prompt);
	    crate::admin::users::delete_empty_lines(path.clone());
	    return;
	} else {
	    append_exercise_to_homework_file(path.clone(), true);
	    crate::admin::users::delete_empty_lines(path.clone());
	    interchange_lines(path, length, line_number-1);
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nEjercicio insertado.\n";
	    println!("{}", prompt);
	    return;
	}
    }

    pub fn duplicate_homework_file() {
	let mut prompt = "\nPrimero necesitamos la ubicación y el nombre del archivo a copiar.";
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	let origin_path = get_user_and_filename();
	let origin_parts: Vec<&str> = origin_path
	    .split("/")
	    .collect();
	std::thread::sleep(std::time::Duration::new(1, 0));
	prompt = "\nAhora necesitamos el usuario donde se va a copiar el archivo.";
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	let mut destination_user = String::new();
	loop {
	    prompt = "\nPor favor introduce \"s\" para salir, o indica el nombre del usuario:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut destination_user) {
		Ok(_) => (),
		Err(error) => {
		    std::thread::sleep(std::time::Duration::new(1, 0));
		    println!("Hubo un error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    if destination_user.trim().to_lowercase() == "s" {
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nSaliendo del sistema.\n");
		std::thread::sleep(std::time::Duration::new(1, 0));
		std::process::exit(0);
	    }
	    destination_user = destination_user.trim().to_string();
	    break;
	}
	let destination_path = format!("users/{}/homeworks/{}", destination_user, origin_parts.last().unwrap());
	std::fs::copy(origin_path, destination_path).unwrap();
	std::thread::sleep(std::time::Duration::new(1, 0));
	println!("\nArchivo copiado.");
    }
}

pub mod groups {
    pub fn list_groups() {
	let mut prompt: String;
	prompt = "\nSe muestra la lista de los grupos creados:\n".to_string();
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	let groups_lines: Vec<&str> = groups_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .collect();
	for (i, group) in groups_lines.iter().enumerate() {
	    prompt = format!("  {}. {}", i+1, group);
	    println!("{}", prompt);
	}
    }

    // retorna "true" si el grupo no existe
    pub fn new_group(group: String) -> bool {
	let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	let groups_lines: Vec<&str> = groups_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .collect();
	for line in groups_lines {
	    if group == line.to_string() {
		return false;
	    }
	}

	true
    }

    pub fn create_group() {
	loop {
	    let mut group = String::new();
	    let mut prompt = "\nPor favor indica el nombre del nuevo grupo:".to_string();
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    prompt = format!("Hubo un error leyendo el nombre: {}", error);
		    println!("{}", prompt);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    if new_group(group.clone()) {
		crate::session::append_to_file("groups.txt".to_string(), group.to_string());
		prompt = format!("\nGrupo {} creado.", group);
		println!("{}", prompt);
		break;
	    } else {
		prompt = format!("\nEl grupo {} ya existe.", group);
		println!("{}", prompt);
	    }
	    std::thread::sleep(std::time::Duration::new(1, 0));
	}
    }

    pub fn delete_group() {
	loop {
	    let mut group = String::new();
	    let mut prompt = "\nPor favor indica el nombre del grupo a eliminar:".to_string();
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    prompt = format!("Hubo un error leyendo el nombre: {}", error);
		    println!("{}", prompt);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    if new_group(group.clone()) {
		std::thread::sleep(std::time::Duration::new(1, 0));
		prompt = format!("\nEl grupo {} no existe.", group);
		println!("{}", prompt);
		break;
	    } else {
		let groups_file = std::fs::read_to_string("groups.txt").unwrap();
		let groups_lines: Vec<String> = groups_file
		    .split("\n")
		    .map(|it| it.to_string())
		    .collect();
		crate::session::create_file("tmp_groups.txt".to_string());
		for line in groups_lines {
		    let parts: Vec<String> = line
			.split(";")
			.map(|it| it.to_string())
			.collect();
		    if parts[0] != group {
			crate::session::append_to_file("tmp_groups.txt".to_string(), line);
		    }
		}
		std::fs::rename("tmp_groups.txt", "groups.txt").unwrap();
		super::users::delete_empty_lines("groups.txt".to_string());

		let users_file = std::fs::read_to_string("users.txt").unwrap();
		let users_lines: Vec<String> = users_file
		    .split("\n")
		    .filter(|it| !it.is_empty())
		    .map(|it| it.to_string())
		    .collect();
		crate::session::create_file("tmp_users.txt".to_string());
		crate::session::append_to_file("tmp_users.txt".to_string(), users_lines[0].clone());
		for line in &users_lines[1..] {
		    let parts: Vec<&str> = line
			.split(";")
			.collect();
		    if parts[2] == group {
			let modified_line = format!("{};{}", parts[0], parts[1]);
			crate::session::append_to_file("tmp_users.txt".to_string(), modified_line.to_string());
		    } else {
			crate::session::append_to_file("tmp_users.txt".to_string(), line.to_string());
		    }
		}
		std::fs::rename("tmp_users.txt", "users.txt").unwrap();
		std::thread::sleep(std::time::Duration::new(1, 0));
		println!("\nGrupo {} eliminado.", group);
		break;		
	    }
	}
    }

    pub fn include_user_in_group() {
	let mut user = String::new();
	let mut group = String::new();
	let mut prompt: &str;
	loop {
	    prompt = "\nPor favor indique el usuario:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut user) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el usuario: {}", error);
		    continue;
		},
	    }
	    user = user.trim().to_string();
	    if super::users::validate_new_user(user.clone()) {
		std::thread::sleep(std::time::Duration::new(1, 0));
		prompt = "\nEl usuario indicado no existe.";
		println!("{}", prompt);
		std::thread::sleep(std::time::Duration::new(1, 0));
		continue;
	    }

	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = "\nIndica el nuevo grupo para el usuario:\n";
	    println!("{}", prompt);
	    let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	    let groups_lines: Vec<&str> = groups_file
		.split("\n")
		.filter(|it| !it.is_empty())
		.collect();
	    for (i, group) in groups_lines.iter().enumerate() {
		println!("{}", format!("  {}. {}", i+1, group));
	    }
	    prompt = "\nOpción:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el grupo: {}", error);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    match group.parse::<usize>() {
		Ok(w) if w <= groups_lines.len() => {
		    group = groups_lines[w-1].to_string();
		    break;
		},
		Ok(_) => {
		    println!("\nLa opción indicada no está entre las opciones.");
		    continue;
		},
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}", error);
		    continue;
		},
	    }
	}
	let users_file = std::fs::read_to_string("users.txt").unwrap();
	let users_lines: Vec<String> = users_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .map(|it| it.to_string())
	    .collect();
	crate::session::create_file("tmp_users.txt".to_string());
	for line in users_lines {
	    let parts: Vec<&str> = line
		.split(";")
		.collect();
	    if parts[0].to_string() == user {
		let data = format!("{};{};{}", parts[0], parts[1], group);
		crate::session::append_to_file("tmp_users.txt".to_string(), data);
	    } else {
		crate::session::append_to_file("tmp_users.txt".to_string(), line.to_string());
	    }
        }
	std::fs::rename("tmp_users.txt", "users.txt").unwrap();
	std::thread::sleep(std::time::Duration::new(1, 0));
	prompt = "\nAsignación de grupo realizada.";
	println!("{}", prompt);
    }

    fn get_vector_of_users(group: String) -> Vec<String> {
	let users_file = std::fs::read_to_string("users.txt")
	    .unwrap();
	let users_lines: Vec<String> = users_file
	    .split("\n")
	    .filter(|it| !it.is_empty())
	    .map(|it| it.to_string())
	    .collect();
	let mut users_vector = vec![];
	for line in &users_lines[1..] {
	    let parts: Vec<String> = line
		.split(";")
		.map(|it| it.to_string())
		.collect();
	    if parts[2] == group {
		users_vector.push(parts[0].clone());
	    }
	}

	users_vector
    }
    
    pub fn list_users_in_a_group() {
	let mut group = String::new();
	let mut prompt = "\nIndica el nuevo grupo para el usuario:\n";
	loop {
	    println!("{}", prompt);
	    let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	    let groups_lines: Vec<&str> = groups_file
		.split("\n")
		.filter(|it| !it.is_empty())
		.collect();
	    for (i, group) in groups_lines.iter().enumerate() {
		println!("{}", format!("  {}. {}", i+1, group));
	    }
	    prompt = "\nOpción:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el grupo: {}", error);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    match group.parse::<usize>() {
		Ok(w) if w <= groups_lines.len() => {
		    group = groups_lines[w-1].to_string();
		    break;
		},
		Ok(_) => {
		    println!("\nLa opción indicada no está entre las opciones.");
		    group.clear();
		    continue;
		},
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}", error);
		    continue;
		},
	    }
	}

	std::thread::sleep(std::time::Duration::new(1, 0));
	prompt = "\nUsuarios del grupo:\n";
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	let users_vector = get_vector_of_users(group);
	for line in users_vector {
	    println!("{}", line);
	}
    }

    pub fn assign_homework_to_group() {
	let mut prompt = "\nPrimero necesitamos la ubicación y el nombre del archivo a copiar.";
	println!("{}", prompt);
	std::thread::sleep(std::time::Duration::new(1, 0));
	let origin_path = super::files::get_user_and_filename();
	let origin_parts: Vec<&str> = origin_path
	    .split("/")
	    .collect();
	std::thread::sleep(std::time::Duration::new(1, 0));
	let mut group = String::new();
	prompt = "\nAhora indica el grupo:\n";
	loop {
	    println!("{}", prompt);
	    let groups_file = std::fs::read_to_string("groups.txt").unwrap();
	    let groups_lines: Vec<&str> = groups_file
		.split("\n")
		.filter(|it| !it.is_empty())
		.collect();
	    for (i, group) in groups_lines.iter().enumerate() {
		println!("{}", format!("  {}. {}", i+1, group));
	    }
	    prompt = "\nOpción:";
	    println!("{}", prompt);
	    match std::io::stdin().read_line(&mut group) {
		Ok(_) => (),
		Err(error) => {
		    println!("Error leyendo el grupo: {}", error);
		    continue;
		},
	    }
	    group = group.trim().to_string();
	    match group.parse::<usize>() {
		Ok(w) if w <= groups_lines.len() => {
		    group = groups_lines[w-1].to_string();
		    break;
		},
		Ok(_) => {
		    println!("\nLa opción indicada no está entre las opciones.");
		    continue;
		},
		Err(error) => {
		    println!("Hubo un error leyendo la opción: {}", error);
		    continue;
		},
	    }
	}
	let users_vector = get_vector_of_users(group);
	for destination_user in users_vector {
	    let destination_path = format!("users/{}/homeworks/{}", destination_user, origin_parts.last().unwrap());
	    std::fs::copy(origin_path.clone(), destination_path.clone()).unwrap();
	}
	std::thread::sleep(std::time::Duration::new(1, 0));
	println!("\nArchivos copiados.");
    }
}

