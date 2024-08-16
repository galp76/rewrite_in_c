use std::fs;
use chrono::{ Local, Datelike, Timelike };

fn main() {
    let user = wrapper_rust::authentication::auth();
    let local = Local::now();
//    println!("{:?}", local);
    if local.year() > 2024 || local.month() > 3 /*|| local.day() > 31*/  {
        println!("El período de prueba ha expirado.");
        std::process::exit(1);
    }
    if user == "admin" {
        std::thread::sleep(std::time::Duration::new(1, 0));
        println!("\nContinuamos al módulo de administración de usuarios.");
        std::thread::sleep(std::time::Duration::new(1, 0));
        wrapper_rust::admin::init();
    }
    let mut session_backup = false;
    for item in fs::read_to_string("configuration.txt").unwrap().lines() {
        let mut options = item.split("=");
        if options.next() == Some("session_backup") && options.next() == Some("true") {
            session_backup = true;
            break;
        }
    }

    let mut session = String::new();
    if session_backup {
        let local = Local::now();
        session = format!("users/{}/sessions/{}-{}-{}_{:02}:{:02}.txt", user, local.year(), local.month(), local.day(), local.hour(), local.minute());
        wrapper_rust::session::create_file(session.clone());
    }

    let mut prompt = "\nPor favor indica una de las siguientes opciones:\n\n  1. Práctica.\n  2. Tarea.\n  3. Salir del sistema.\n\nOpción:";
    let mut user_input: usize;
    loop {
        println!("{prompt}");
        if session_backup {
            wrapper_rust::session::append_to_file(session.clone(), prompt.to_string());
        }
        user_input = wrapper_rust::get_user_input(session.clone(), session_backup);
        match user_input {
            1 => {
                prompt = "\nContinuamos al módulo de práctica.";
                println!("{prompt}");
                if session_backup {
                    wrapper_rust::session::append_to_file(session.clone(), prompt.to_string());
                }
                std::thread::sleep(std::time::Duration::new(1, 0));
                wrapper_rust::practice::practice::practice(session.clone(), session_backup, "leon1.8t");
            },
            2 => {
                prompt = "\nContinuamos al módulo de tareas.";
                println!("{prompt}");
                if session_backup {
                    wrapper_rust::session::append_to_file(session.clone(), prompt.to_string());
                }
                std::thread::sleep(std::time::Duration::new(1, 0));
                if wrapper_rust::homework::homework(session.clone(), session_backup, "leon1.8t", user.clone()) == 1 {
                    prompt = "\nPor favor indica una de las siguientes opciones:\n\n  1. Práctica.\n  2. Tarea.\n  3. Salir del sistema.\n\nOpción:";
                    continue;
                }
                std::thread::sleep(std::time::Duration::new(1, 0));
                println!("HAY QUE TERMINAR LA IMPLEMENTACION DEL MODULO DE TAREAS.");
                std::thread::sleep(std::time::Duration::new(1, 0));
                std::process::exit(0);
            },
            3 => {
                prompt = "\nSaliendo del sistema.\n";
                println!("{prompt}");
                if session_backup {
                    wrapper_rust::session::append_to_file(session.clone(), prompt.to_string());
                }
                std::thread::sleep(std::time::Duration::new(1, 0));
                std::process::exit(0);
            },
            _ => {
                std::thread::sleep(std::time::Duration::new(1, 0));
                prompt = "\nOpción no válida.\n\nIndique la opción nuevamente:\n";
                continue; // se usa este "continue" en vez del "println!" de abajo, para que el prompt lo imprima el "println!" del comienzo del loop (linea 30)
        /*		println!("{prompt}");
                if session_backup {
                    wrapper_rust::session::append_to_file(session.clone(), prompt.to_string());
                }*/
            },	
        }
    }
}
