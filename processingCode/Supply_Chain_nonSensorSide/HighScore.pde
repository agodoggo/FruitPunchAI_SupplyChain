import java.util.*;

//global variables
PriorityQueue<Node> highScoreList_ReadOnly = new PriorityQueue<Node>(11, new ScoreComparator_dec()); //top of heap is maximum score 
PriorityQueue<Node> highScoreList_WriteMem = new PriorityQueue<Node>(11, new ScoreComparator_asc());// top of heap is minimum score
int MAX_HIGHSCORES = 1024;
File highScoresFile = dataFile("Data/highScores.txt");
String hS_filePath = highScoresFile.getPath();

public void initHighScore(){
  println("highScores.txt is" + highScoresFile.isFile());
  if(highScoresFile.isFile()){
    String[] pastScores = loadStrings(hS_filePath);
    print("past scores" + pastScores);
    for(int i = 0; i < pastScores.length; i++){
      println("past score: "+ pastScores[i]);
      String[] tmp = split(pastScores[i],",");
      highScoreList_ReadOnly.add(new Node(tmp[0],Integer.parseInt(tmp[1])));
      highScoreList_WriteMem.add(new Node(tmp[0],Integer.parseInt(tmp[1])));
    }
  }
  else{
    hS_filePath = "Data/highScores.txt";
  }
}

public void addHighScore(String name, int score){
  
  while(highScoreList_WriteMem.size()>MAX_HIGHSCORES){
    highScoreList_WriteMem.poll();
  }
  Node tmpNode = new Node(name, score);
  if(highScoreList_WriteMem.size() == MAX_HIGHSCORES){
    if(tmpNode.score>highScoreList_WriteMem.peek().score){
      highScoreList_WriteMem.poll();
      highScoreList_ReadOnly.add(tmpNode);
      highScoreList_WriteMem.add(tmpNode);
      println("Node " + name + "," + score + " added");
    }
  }
  else if(highScoreList_WriteMem.size() < MAX_HIGHSCORES){
      highScoreList_ReadOnly.add(tmpNode);
      highScoreList_WriteMem.add(tmpNode);
      println("Node " + name + "," + score + " added");
  }
}

public void saveHighScore(){
  String[] tmp = new String[highScoreList_WriteMem.size()];
  Node[] tmpNodeList = highScoreList_WriteMem.toArray(new Node[highScoreList_WriteMem.size()]);
  for(int i = 0; i < tmpNodeList.length; i++){
    tmp[i]=tmpNodeList[i].name+","+tmpNodeList[i].score;
    println("entry "+ i + ": " + tmp[i]);
  }
  saveStrings(hS_filePath,tmp);
  println("Strings saved to: " + hS_filePath);
}

public void displayHighScores(){
  PriorityQueue<Node> tmpQueue = highScoreList_ReadOnly;
  Node tmpNode;
  fill(0);
  textSize(26);
  textAlign(LEFT);
  for(int i = 0; i < 5; i++){
    tmpNode = tmpQueue.poll();
    if(tmpNode !=null){
      if(tmpNode.name != null){
          text(tmpNode.name,70,673+120*i);
        }
      if (str(tmpNode.score) != null){
          text(str(tmpNode.score),550,673+120*i);
        } 
      }
    }
  }

class Node{

    String name;        // the name
    int score;       // assuming you're using double

    public Node(String name, int score){
        this.name = name;
        this.score = score;         // assuming negative scores are allowed
    }
    
    public String getname(){
      return name;
    }
    
    public int getscore(){
      return score;
    }
}

class ScoreComparator_dec implements Comparator<Node>{ 
// Overriding compare()method of Comparator for descending order of score
  public int compare(Node n1, Node n2) { 
    if (n1.score < n2.score){
      return 1;
    }
    else if (n1.score > n2.score){
      return -1; 
    }
    return 0; 
  } 
}
class ScoreComparator_asc implements Comparator<Node>{ 
// Overriding compare()method of Comparator for descending order of score
  public int compare(Node n1, Node n2) { 
    if (n1.score < n2.score){
      return -1;
    }
    else if (n1.score > n2.score){
      return 1; 
    }
    return 0; 
  } 
}
