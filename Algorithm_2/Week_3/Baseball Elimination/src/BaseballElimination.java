import edu.princeton.cs.algs4.Bag;
import edu.princeton.cs.algs4.FlowEdge;
import edu.princeton.cs.algs4.FlowNetwork;
import edu.princeton.cs.algs4.FordFulkerson;
import edu.princeton.cs.algs4.In;
import edu.princeton.cs.algs4.StdOut;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

public class BaseballElimination {
    // *** *** *** *** *** Private Attributes *** *** *** *** *** //
    private HashMap<String, TeamRecord_> teamsRecord_;
    private TeamsSchedule_ teamsSchedule_;

    // *** *** *** *** *** Private Classes *** *** *** *** *** //

    private static class TeamRecord_ implements Comparable<TeamRecord_> {
        // Please refer to TeamsInfo_ class for the reason behind assigning a number to
        // each team.
        int teamNumber_;
        private int numberOfWins_;
        private int numberOfLosses_;
        private int numberOfRemainingGames_;

        public int compareTo(TeamRecord_ that) {
            return Integer.compare(teamNumber_, that.teamNumber_);
        }
    }

    private static class TeamsSchedule_ {
        // The game schedule matrix is symmetric. Therefore, we do not need to store all the data.
        // We assign numbers to the teams:
        //        i  team         wins loss left   Atl Phi NY  Mon
        //------------------------------------------------
        //                0  Atlanta       83   71    8     -   1   6   1
        //                1  Philadelphia  80   79    3     1   -   0   2
        //                2  New York      78   78    6     6   0   -   0
        //                3  Montreal      77   82    3     1   2   0   -
        // For team0 (Atlanta) we store the array {1   6   1}
        // For team1 (Philadelphia) we store the array {0   2}
        private HashMap<Integer, int[]> teamsGames_ = new HashMap<>();

        private int getNumberOfGamesBetweenTwoTeams(Integer team1Number, Integer team2Number) {
            // Since not all the data is stored, only team with the smaller number has information about the number
            // of games between two given teams.
            switch (team1Number.compareTo(team2Number)) {
                case 0:
                    return 0;
                case 1: {
                    int[] teamGames = teamsGames_.get(team2Number);
                    return teamGames[team1Number - team2Number - 1];
                }
                case -1: {
                    int[] teamGames = teamsGames_.get(team1Number);
                    return teamGames[team2Number - team1Number - 1];
                }
                default:
                    throw new IllegalArgumentException("Team number is not valid!");
            }
        }
    }

    private class FlowNetworkBuilder_ {
        int numberOfOtherTeams_;
        HashMap<Integer, Integer> teamToVertex_;

        TeamRecord_ queryTeamRecord_;
        FlowNetwork flowNetwork_;
        FordFulkerson fordFulkerson_;

        private FlowNetworkBuilder_(TeamRecord_[] otherTeamsRecord, TeamRecord_ queryTeamRecord) {
            Arrays.sort(otherTeamsRecord);
            numberOfOtherTeams_ = otherTeamsRecord.length;
            queryTeamRecord_ = queryTeamRecord;
            int numberOfMatches = (numberOfOtherTeams_) * (numberOfOtherTeams_ - 1) / 2;
            int numberOfVertices = numberOfMatches + numberOfOtherTeams_ + 2;
            flowNetwork_ = new FlowNetwork(numberOfVertices);
            teamToVertex_ = new HashMap<>();

            assignTeamVertices(otherTeamsRecord);
            assignGameVertices_(otherTeamsRecord);

            fordFulkerson_ = new FordFulkerson(flowNetwork_, 0, flowNetwork_.V() - 1);
        }

        private void assignTeamVertices(TeamRecord_[] otherTeamsRecord) {
            int targetVertexNumber = flowNetwork_.V() - 1;

            // Starting from the first team vertex
            int teamVertex = flowNetwork_.V() - numberOfOtherTeams_ - 1;
            int numberOfPossibleWinsForQueryTeam =
                    queryTeamRecord_.numberOfWins_ + queryTeamRecord_.numberOfRemainingGames_;
            for (TeamRecord_ record : otherTeamsRecord) {
                int capacity = numberOfPossibleWinsForQueryTeam - record.numberOfWins_;
                FlowEdge flowEdge = new FlowEdge(teamVertex, targetVertexNumber, capacity);
                flowNetwork_.addEdge(flowEdge);
                teamToVertex_.put(record.teamNumber_, teamVertex);
                teamVertex++;
            }
        }

        private void assignGameVertices_(TeamRecord_[] otherTeamsRecord) {
            // The first vertex associated to a game is right after the source vertex.
            int sourceVertexNumber = 0;
            int gameVertex = 1;
            for (int team1Index = 0; team1Index < numberOfOtherTeams_; team1Index++) {
                TeamRecord_ record1 = otherTeamsRecord[team1Index];
                for (int team2Index = team1Index + 1; team2Index < numberOfOtherTeams_; team2Index++) {
                    // Connect the game vertex to the source.
                    TeamRecord_ record2 = otherTeamsRecord[team2Index];
                    int capacity = teamsSchedule_.getNumberOfGamesBetweenTwoTeams(record1.teamNumber_, record2.teamNumber_);
                    FlowEdge gameEdge = new FlowEdge(sourceVertexNumber, gameVertex, capacity);
                    flowNetwork_.addEdge(gameEdge);

                    // Connect the game vertex to the corresponding team vertices.
                    FlowEdge infiniteEdge1 = new FlowEdge(
                            gameVertex, teamToVertex_.get(record1.teamNumber_), Double.POSITIVE_INFINITY);
                    FlowEdge infiniteEdge2 = new FlowEdge(
                            gameVertex, teamToVertex_.get(record2.teamNumber_), Double.POSITIVE_INFINITY);
                    flowNetwork_.addEdge(infiniteEdge1);
                    flowNetwork_.addEdge(infiniteEdge2);
                    gameVertex++;
                }
            }
        }

        private Boolean areAllSourceEdgesFull_() {
            int sourceVertexNumber = 0;
            Iterable<FlowEdge> sourceEdges = flowNetwork_.adj(sourceVertexNumber);
            for (FlowEdge edge : sourceEdges) {
                if (edge.capacity() != edge.flow())
                    return true;
            }

            return false;
        }

        private Iterable<String> getCertificateOfElimination_() {
            Bag<String> teams = new Bag<>();
            for (Map.Entry teamAndRecord : teamsRecord_.entrySet()) {
                String teamName = (String) teamAndRecord.getKey();
                TeamRecord_ teamRecord = (TeamRecord_) teamAndRecord.getValue();
                if (teamRecord.teamNumber_ == queryTeamRecord_.teamNumber_)
                    continue;
                
                int vertexNumber = teamToVertex_.get(teamRecord.teamNumber_);
                if (fordFulkerson_.inCut(vertexNumber))
                    teams.add(teamName);
            }

            return teams;
        }
    }

    // *** *** *** *** *** Private Methods *** *** *** *** *** //

    private void assignTeam_(In in, int teamNumber) {
        String teamName = in.readString();
        TeamRecord_ teamRecord = new TeamRecord_();
        teamRecord.teamNumber_ = teamNumber;
        teamRecord.numberOfWins_ = in.readInt();
        teamRecord.numberOfLosses_ = in.readInt();
        teamRecord.numberOfRemainingGames_ = in.readInt();
        teamsRecord_.put(teamName, teamRecord);
    }

    private void assignTeamInfo_(In in, int numberOfTeams, int teamNumber) {
        ArrayList<Integer> numberOfGamesAgainstOtherTeams = new ArrayList<>();
        for (int otherTeamIndex = 0; otherTeamIndex < numberOfTeams; otherTeamIndex++) {
            numberOfGamesAgainstOtherTeams.add(in.readInt());
        }

        // Depending on the team number, we initialize the array to hold the information
        // for games against other teams. Refer to TeamsInfo_.
        int informationArraySize = numberOfTeams - (teamNumber + 1);
        int[] gamesAgainstOtherTeams = new int[informationArraySize];
        for (int index = 0; index < informationArraySize; index++) {
            gamesAgainstOtherTeams[index] = numberOfGamesAgainstOtherTeams.get(index + teamNumber + 1);
        }

        teamsSchedule_.teamsGames_.put(teamNumber, gamesAgainstOtherTeams);
    }

    private void readFileAndStoreData_(String fileName) {
        // A valid file looks like below:
        //        4
        //        Atlanta       83 71  8  0 1 6 1
        //        Philadelphia  80 79  3  1 0 0 2
        //        New_York      78 78  6  6 0 0 0
        //        Montreal      77 82  3  1 2 0 0
        In in = new In(fileName);
        int numberOfTeams = in.readInt();
        int teamNumber = 0;
        while (!in.isEmpty()) {
            assignTeam_(in, teamNumber);
            assignTeamInfo_(in, numberOfTeams, teamNumber);
            teamNumber++;
        }
    }

    private TeamRecord_[] getOtherTeamsRecord_(TeamRecord_ queryTeamRecord) {
        int n = numberOfTeams();
        TeamRecord_[] otherTeamsRecord = new TeamRecord_[n - 1];

        int counter = 0;
        for (TeamRecord_ record : teamsRecord_.values()) {
            if (queryTeamRecord.teamNumber_ == record.teamNumber_)
                continue;
            otherTeamsRecord[counter] = record;
            counter++;
        }

        return otherTeamsRecord;
    }

    private int triviallyEliminatedBy_(TeamRecord_[] otherTeamsRecord, TeamRecord_ queryTeamRecord) {
        // Trivial elimination.
        int maxNumberOfPossibleWins = queryTeamRecord.numberOfWins_ + queryTeamRecord.numberOfRemainingGames_;
        for (TeamRecord_ record : otherTeamsRecord) {
            if (maxNumberOfPossibleWins < record.numberOfWins_)
                return record.teamNumber_;
        }

        return -1;
    }

    private void validateTeam_(String team) {
        if (team == null) {
            throw new IllegalArgumentException("team is null!");
        }
    }

    private void validateTeamRecord_(TeamRecord_ teamRecord) {
        if (teamRecord == null) {
            throw new IllegalArgumentException("team does not exist!");
        }
    }

    // *** *** *** *** *** Public Methods *** *** *** *** *** //

    // create a baseball division from given filename in format specified below
    public BaseballElimination(String filename) {
        if (filename == null) {
            throw new IllegalArgumentException("filename is null!");
        }

        teamsRecord_ = new HashMap<>();
        teamsSchedule_ = new TeamsSchedule_();
        readFileAndStoreData_(filename);
    }

    // number of teams
    public int numberOfTeams() {
        return teamsRecord_.size();
    }

    // all teams
    public Iterable<String> teams() {
        return teamsRecord_.keySet();
    }

    // number of wins for given team
    public int wins(String team) {
        validateTeam_(team);
        TeamRecord_ teamRecord = teamsRecord_.get(team);
        validateTeamRecord_(teamRecord);
        return teamRecord.numberOfWins_;
    }

    // number of losses for given team
    public int losses(String team) {
        validateTeam_(team);
        TeamRecord_ teamRecord = teamsRecord_.get(team);
        validateTeamRecord_(teamRecord);
        return teamRecord.numberOfLosses_;
    }

    // number of remaining games for given team
    public int remaining(String team) {
        validateTeam_(team);
        TeamRecord_ teamRecord = teamsRecord_.get(team);
        validateTeamRecord_(teamRecord);
        return teamRecord.numberOfRemainingGames_;
    }

    // number of remaining games between team1 and team2
    public int against(String team1, String team2) {
        validateTeam_(team1);
        validateTeam_(team2);
        TeamRecord_ team1Record = teamsRecord_.get(team1);
        TeamRecord_ team2Record = teamsRecord_.get(team2);
        validateTeamRecord_(team1Record);
        validateTeamRecord_(team2Record);

        int numberOfTeam1 = team1Record.teamNumber_;
        int numberOfTeam2 = team2Record.teamNumber_;
        return teamsSchedule_.getNumberOfGamesBetweenTwoTeams(numberOfTeam1, numberOfTeam2);
    }

    // is given team eliminated?
    public boolean isEliminated(String team) {
        validateTeam_(team);
        TeamRecord_ queryTeamRecord = teamsRecord_.get(team);
        validateTeamRecord_(queryTeamRecord);

        TeamRecord_[] otherTeamsRecord = getOtherTeamsRecord_(queryTeamRecord);
        if (triviallyEliminatedBy_(otherTeamsRecord, queryTeamRecord) >= 0)
            return true;

        FlowNetworkBuilder_ flowNetworkBuilder = new FlowNetworkBuilder_(otherTeamsRecord, queryTeamRecord);
        //System.out.println(flowNetworkBuilder.flowNetwork_.toString());
        return flowNetworkBuilder.areAllSourceEdgesFull_();
    }

    // subset R of teams that eliminates given team; null if not eliminated
    public Iterable<String> certificateOfElimination(String team) {
        validateTeam_(team);
        TeamRecord_ queryTeamRecord = teamsRecord_.get(team);
        validateTeamRecord_(queryTeamRecord);

        TeamRecord_[] otherTeamsRecord = getOtherTeamsRecord_(queryTeamRecord);

        // Get the team which might eliminate the query team trivially.
        int teamNumber = triviallyEliminatedBy_(otherTeamsRecord, queryTeamRecord);
        if (teamNumber >= 0) {
            for (Map.Entry teamAndRecord : teamsRecord_.entrySet()) {
                String teamName = (String) teamAndRecord.getKey();
                TeamRecord_ teamRecord = (TeamRecord_) teamAndRecord.getValue();
                if (teamRecord.teamNumber_ == teamNumber) {
                    Bag<String> teams = new Bag<>();
                    teams.add(teamName);
                    return teams;
                }
            }
        }

        FlowNetworkBuilder_ flowNetworkBuilder = new FlowNetworkBuilder_(otherTeamsRecord, queryTeamRecord);
        // Team is not eliminated.
        if (!flowNetworkBuilder.areAllSourceEdgesFull_())
            return null;

        return flowNetworkBuilder.getCertificateOfElimination_();
    }

    // Test
    public static void main(String[] args) {
        BaseballElimination be = new BaseballElimination(args[0]);
        StdOut.printf("Number of teams: %d\n", be.numberOfTeams());
        StdOut.printf("Wins: %d\n", be.wins("Montreal"));
        StdOut.printf("Losses: %d\n", be.losses("Montreal"));
        StdOut.printf("Remaining: %d\n", be.remaining("Montreal"));

        StdOut.printf("against: %d\n", be.against("Montreal", "Atlanta"));
        StdOut.printf("against: %d\n", be.against("Montreal", "Philadelphia"));
        StdOut.printf("against: %d\n", be.against("Montreal", "New_York"));
        StdOut.printf("against: %d\n", be.against("Montreal", "Montreal"));
        be.certificateOfElimination("Montreal");

        System.out.println(be.isEliminated("Montreal"));
    }
}
