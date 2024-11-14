# tests para el comando export
tests = [
    # Pruebas básicas de export
    "export VAR1=hello",  # Asignación simple
    "export VAR2='hello world'",  # Asignación con comillas simples
    "export VAR3=\"hello world\"",  # Asignación con comillas dobles
    "export VAR4=42",  # Asignación numérica
    "export VAR5=",  # Variable vacía
    "export VAR6",  # Exportar variable sin asignar (debe estar vacía)

    # Export de variables con expansión
    "export USERNAME='testuser' && echo $USERNAME",  # Expansión de variable
    "export HOME='/home/testuser' && echo $HOME",  # Expansión con ruta
    "export PATH=$PATH:/new/path && echo $PATH",  # Añadir a variable PATH

    # Export y uso de variables
    "export GREETING='Hello' && echo $GREETING, world!",  # Uso inmediato de variable exportada
    "export NUMBER=123 && echo The number is $NUMBER",  # Uso numérico
    "export EMPTY_VAR='' && echo 'Is empty: '$EMPTY_VAR",  # Variable vacía

    # Export con comillas y caracteres especiales
    "export QUOTE='\"Double Quotes\"' && echo $QUOTE",  # Comillas dobles dentro de comillas simples
    "export SINGLE_QUOTE=\"'Single Quote'\" && echo $SINGLE_QUOTE",  # Comillas simples dentro de comillas dobles
    "export SPECIAL_CHAR='Special_$#@!' && echo $SPECIAL_CHAR",  # Caracteres especiales
    "export SLASH='Path/With/Slash' && echo $SLASH",  # Caracteres de barra

    # Casos con expansión de variables
    "export VAR7='$NOT_DEFINED' && echo $VAR7",  # Variable no definida
    "export VAR8='$HOME' && echo $VAR8",  # Expansión de variable HOME
    "export VAR9='Path: $PATH' && echo $VAR9",  # Expansión dentro de cadena

    # Casos de error
    "export =invalid",  # Asignación inválida
    "export 123INVALID=error",  # Nombre de variable inválido (empieza con número)
    "export VAR10-INVALID=value",  # Nombre de variable inválido (guión)
    "export VAR11=NoSpaces Allowed",  # Error de espacio sin comillas
    "export VAR12==double_equals",  # Doble igual inválido
    "export ''=empty_name",  # Nombre vacío
    "export \"invalid\"=error",  # Nombre entre comillas

    # Pruebas de sobreescritura
    "export VAR13='initial' && echo $VAR13 && export VAR13='updated' && echo $VAR13",  # Sobreescritura de valor
    "export VAR14='one' && export VAR14='two' && echo $VAR14",  # Sobreescribir sin uso intermedio

    # Export de variables existentes
    "export PATH && echo $PATH",  # Exportar variable de entorno existente
    "export USER && echo $USER",  # Exportar usuario
    "export SHELL && echo $SHELL",  # Exportar variable SHELL

    # Uso con redirecciones y tuberías
    "export PIPE_VAR='Hello' | echo $PIPE_VAR",  # Export con tubería (no debe expandirse)
    "export FILE_VAR='Output' > output.txt && cat output.txt",  # Export y redirección
    "export VAR15='Line1\nLine2' && echo -e $VAR15",  # Secuencia de escape en variable exportada

    # Export y unset
    "export DELETE_ME='To be removed' && echo $DELETE_ME && unset DELETE_ME && echo $DELETE_ME",  # Export y luego eliminar
    "unset NOT_DEFINED && export STILL_WORKS='Yes' && echo $STILL_WORKS",  # Unset de variable no definida seguido de export

    # Casos extremos y especiales
    "export LONG_VAR='A'*1000 && echo ${#LONG_VAR}",  # Export de variable muy larga
    "export EMPTY='' && export EMPTY && echo 'Empty variable exported'",  # Export de variable vacía y exportarla de nuevo
    "export UPPERCASE_VAR=UPPER && export lowercase_var=lower && echo $UPPERCASE_VAR $lowercase_var",  # Variables con diferentes casos
]
