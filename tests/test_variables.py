# tests para manejo y expansión de variables
tests = [
    # Pruebas básicas de expansión
    "export VAR1=hello && echo $VAR1",  # Expansión simple
    "export VAR2='world' && echo $VAR2",  # Expansión con comillas
    "export VAR3='hello world' && echo $VAR3",  # Expansión con espacios
    "export VAR4='\"quoted\"' && echo $VAR4",  # Expansión con comillas internas

    # Expansión con variables no definidas
    "echo $UNDEFINED_VAR",  # Variable no definida
    "export EMPTY='' && echo $EMPTY",  # Variable vacía
    "unset VAR1 && echo $VAR1",  # Mostrar una variable después de eliminarla

    # Expansión con variables especiales
    "echo $$",  # PID del shell
    "echo $?",  # Último código de salida
    "export VAR=42 && echo $? && unset VAR && echo $?",  # Verificar código de salida después de unset
    "echo $0",  # Nombre del script

    # Expansión combinada
    "export A=1 B=2 C=3 && echo $A$B$C",  # Concatenación de variables
    "export PREFIX='hello' && export SUFFIX='world' && echo $PREFIX-$SUFFIX",  # Concatenar variables con texto
    "export PATH_VAR='/usr/bin' && echo $PATH_VAR:$PATH",  # Mostrar variable junto con $PATH

    # Pruebas con comillas
    "export QUOTED='\"double quoted\"' && echo $QUOTED",  # Expansión con comillas dobles
    "export SINGLE='\'single quoted\'' && echo $SINGLE",  # Expansión con comillas simples
    "export MIXED=\"hello'world\" && echo $MIXED",  # Expansión con comillas mezcladas

    # Variables en contexto de comandos
    "export VAR_CMD='echo command' && $VAR_CMD",  # Usar variable como comando
    "export VAR_ARG='hello world' && echo $VAR_ARG",  # Usar variable como argumento
    "export GREP_CMD='grep VAR' && echo 'test VAR' | $GREP_CMD",  # Usar variable en una tubería

    # Variables en redirecciones
    "export OUTFILE='output.txt' && echo 'test' > $OUTFILE && cat $OUTFILE",  # Redirigir a un archivo usando variable
    "export INFILE='input.txt' && echo 'content' > $INFILE && cat < $INFILE",  # Usar variable para redirección de entrada
    "export CMD='cat' && export FILE='input.txt' && $CMD < $FILE",  # Usar variables para comando y archivo

    # Expansión en diferentes contextos
    "echo '$VAR1'",  # No expandir dentro de comillas simples
    "echo \"$VAR2\"",  # Expandir dentro de comillas dobles
    "echo text$VAR3text",  # Expandir en medio de texto
    "echo ${VAR4}suffix",  # Usar llaves para expansión

    # Variables numéricas y especiales
    "export NUM_VAR=123 && echo $NUM_VAR",  # Variable con números
    "export _UNDERSCORE_VAR=underscore && echo $_UNDERSCORE_VAR",  # Variable con guion bajo
    "export VAR@SPECIAL=special && echo $VAR@SPECIAL",  # Variable con carácter especial (debe fallar)

    # Variables y comandos integrados
    "export PWD_OVERRIDE='/custom/path' && cd $PWD_OVERRIDE && pwd && unset PWD_OVERRIDE",  # Usar variable para cd
    "export VAR='hello' && unset VAR && export VAR='new_value' && echo $VAR",  # Reasignar después de unset
    "export VAR=1 && export VAR=$((VAR + 1)) && echo $VAR",  # Incremento usando expansión (si es compatible)

    # Pruebas con múltiples variables y tuberías
    "export A=hello B=world && echo $A | grep $B",  # Usar variables en tubería
    "export STR='a b c' && echo $STR | tr ' ' '-'",  # Usar variable con comando externo
    "export LIST='1 2 3' && for i in $LIST; do echo $i; done",  # Iterar sobre variable

    # Casos extremos
    "export VAR='*' && echo $VAR",  # Expansión con carácter comodín
    "export VAR='$' && echo $VAR",  # Expansión con símbolo de dólar
    "export VAR=' ' && echo $VAR",  # Expansión con espacio
    "export VAR='' && echo ${VAR:-default}",  # Valor predeterminado si la variable está vacía

    # Variables reservadas (no deben ser sobrescritas)
    "export PATH='/tmp' && echo $PATH && unset PATH",  # Intentar sobrescribir PATH
    "export HOME='/tmp' && cd && pwd && unset HOME",  # Intentar sobrescribir HOME
    "export SHELL='/bin/fake_shell' && echo $SHELL && unset SHELL",  # Intentar sobrescribir SHELL
]
