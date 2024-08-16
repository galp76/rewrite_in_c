pub mod practice {
    pub fn practice(session_file: String, session_backup: bool, password: &str) {
	let (descriptions, programs) = crate::get_options();
	crate::display_options(&descriptions, true, session_file.clone(), session_backup);
	let mut prompt: String;
	let mut user_input: usize;
	loop {
	    user_input = crate::get_user_input(session_file.clone(), session_backup);
	    if user_input != 0 && user_input <= programs.len() {
		crate::launch_program(user_input, programs.clone(), session_file.clone(), session_backup, password);
	    } else {
		std::thread::sleep(std::time::Duration::new(1, 0));
		prompt = "\nOpción no válida.\n\nIndique la opción nuevamente:\n".to_string();
		crate::session::print_and_save(prompt, session_file.clone(), session_backup);
		std::thread::sleep(std::time::Duration::new(1, 0));
		crate::display_options(&descriptions, false, session_file.clone(), session_backup);
		continue;
	    }
	    std::thread::sleep(std::time::Duration::new(1, 0));
	    prompt = format!("\n{}", std::iter::repeat("*").take(105).collect::<String>());
	    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	    prompt = "\nSelecciona una opción para continuar con otro ejercicio, o introduce la letra \"s\" para salir del sistema.\n".to_string();
	    crate::session::print_and_save(prompt, session_file.clone(), session_backup);
	    crate::display_options(&descriptions, false, session_file.clone(), session_backup);
	}
    }
}
