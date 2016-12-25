name := "using-pi4j"

version := "dev"

libraryDependencies += "com.pi4j" % "pi4j-core" % "1.1"
libraryDependencies += "com.lihaoyi" % "ammonite" % "0.8.1" cross CrossVersion.full

com.github.retronym.SbtOneJar.oneJarSettings

initialCommands in console := """ammonite.Main().run()"""
initialCommands in (Test, console) := """ammonite.Main().run()"""
