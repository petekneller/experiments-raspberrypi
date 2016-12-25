name := "using-pi4j"

version := "dev"

libraryDependencies += "com.pi4j" % "pi4j-core" % "1.1"
libraryDependencies += "com.lihaoyi" % "ammonite" % "0.8.1" % "test" cross CrossVersion.full

com.github.retronym.SbtOneJar.oneJarSettings

initialCommands in (Test, console) := """ammonite.Main().run()"""
