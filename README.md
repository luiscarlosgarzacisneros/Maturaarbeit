# Maturaarbeit
Das ist das Repository für meine Maturaarbeit "Entwickeln von Brettspielalgorithmen".

In den Files werden Sie in den Klassen, die nach den entsprechenden Spielen benannt worden sind (VierGewinnt, Dame und Schach) eine play-Methode finden. Dort können Sie die zwei Spielertypen definieren, indem Sie die Klassen der player_1- und player_2-Instanzen auswählen (MCTSPlayer, MinimaxPlayer oder HumanPlayer). Es wird der MinimaxPlayer empfohlen. In der spielen(anzahl)-Funktion können Sie die Anzahl Partien (oder Simulationen) als Parameter definieren.

Die _tk-Versionen der VierGewinnt- und Dame-Programme verwenden tkinter für die Spielbrettdarstellung. In der Regel ist tkinter in  Python-Installationen schon erhalten. Sie können mit dem command "python -m tkinter" nachschauen, ob Sie tkinter installiert haben.

Bei den C++-Programme wurde der GNU g++ 13.1.0 compiler gebraucht. (Es wurden nur standard libraries verwendet.)

