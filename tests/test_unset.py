# tests para el comando unset
tests = [
    # Pruebas básicas
    "export VAR1='Hello World' && echo $VAR1 && unset VAR1 && echo $VAR1",  # Crear, mostrar y eliminar una variable
    "export MY_VAR=42 && unset MY_VAR && echo $MY_VAR",  # Crear y eliminar variable, luego intentar mostrar

    # Eliminar múltiples variables a la vez
    "export VAR1=one VAR2=two VAR3=three && unset VAR1 VAR2 && echo $VAR1 $VAR2 $VAR3",  # Dejar VAR3 intacta
    "export A=1 B=2 C=3 && unset A B C && echo $A $B $C",  # Eliminar todas las variables

    # Intentar eliminar variables no existentes
    "unset NON_EXISTENT_VAR && echo $NON_EXISTENT_VAR",  # No debe generar error
    "export TEST_VAR='test' && unset TEST_VAR && unset TEST_VAR && echo $TEST_VAR",  # Eliminar dos veces una variable

    # Comportamiento al eliminar variables del entorno estándar
    "unset PATH && echo $PATH",  # Eliminar PATH y verificar si se elimina correctamente
    "unset HOME && cd && pwd",  # Eliminar HOME y verificar si `cd` sigue funcionando
    "unset SHELL && echo $SHELL",  # Eliminar SHELL y mostrar su valor

    # Interacción con otros comandos
    "export PWD_OVERRIDE='/custom/path' && unset PWD_OVERRIDE && pwd",  # Eliminar variable y verificar comportamiento de pwd
    "export VAR=unset_test && echo $VAR && unset VAR && env | grep VAR",  # Eliminar variable y verificar con env

    # Uso con redirecciones y tuberías
    "export TEMP_VAR=temp && unset TEMP_VAR > output.txt && cat output.txt",  # Redirigir salida del comando unset
    "export PIPE_VAR=pipe_test && unset PIPE_VAR | grep pipe_test",  # Uso con tuberías, debe no generar salida

    # Pruebas con comillas
    "export QUOTED_VAR='This is a test' && echo $QUOTED_VAR && unset QUOTED_VAR && echo $QUOTED_VAR",  # Eliminar variable con valor entre comillas
    "export DOUBLE_QUOTED_VAR=\"Double Test\" && unset DOUBLE_QUOTED_VAR && echo $DOUBLE_QUOTED_VAR",  # Eliminar variable con comillas dobles

    # Casos extremos
    "unset",  # Llamar a unset sin argumentos
    "unset '' && unset \"\"",  # Intentar eliminar variables vacías o con nombres inválidos
    "export VAR='test' && unset VAR && export VAR='new_value' && echo $VAR",  # Reasignar después de eliminar

    # Pruebas de compatibilidad con export
    "export ABC=123 && unset ABC && export ABC=456 && echo $ABC",  # Verificar si export funciona después de unset
    "export VAR1=val1 && export VAR2=val2 && unset VAR1 VAR2 && export VAR1=val3 && echo $VAR1 $VAR2",  # Eliminar y reasignar variables

    # Eliminar variables reservadas (no debería afectar el shell)
    "unset PWD && echo $PWD",  # Verificar si PWD sigue mostrándose después de eliminar
    "unset OLDPWD && echo $OLDPWD",  # Verificar si OLDPWD sigue existiendo
    "unset USER && echo $USER",  # Intentar eliminar la variable USER

    # Pruebas con variables numéricas
    "export 123VAR=numeric && echo $123VAR && unset 123VAR && echo $123VAR",  # Intentar eliminar una variable con nombre inválido
    "export _UNDERSCORE_VAR=underscore && echo $_UNDERSCORE_VAR && unset _UNDERSCORE_VAR && echo $_UNDERSCORE_VAR",  # Eliminar variable con guion bajo

    # Eliminar una variable mientras se usa
    "export ACTIVE_VAR=active && (unset ACTIVE_VAR && echo $ACTIVE_VAR)",  # Eliminar en subshell
    "export LOOP_VAR=loop && for i in 1 2 3; do unset LOOP_VAR && echo $LOOP_VAR; done",  # Eliminar en bucle

    # Pruebas con variables especiales
    "unset $? && echo $?",  # Intentar eliminar la variable de estado de salida
    "unset $$ && echo $$",  # Intentar eliminar el PID del shell
    "unset $0 && echo $0",  # Intentar eliminar el nombre del script
]
