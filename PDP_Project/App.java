package com.predictionmarketing.RecommendApp;

import java.io.File;
import java.lang.annotation.Target;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Dictionary;
import java.util.List;
import java.util.Random;
//import java.util.Vector;

import org.apache.commons.lang.math.RandomUtils;
import org.apache.mahout.classifier.sgd.AdaptiveLogisticRegression;
import org.apache.mahout.classifier.sgd.CrossFoldLearner;
import org.apache.mahout.classifier.sgd.L2;
import org.apache.mahout.classifier.sgd.OnlineLogisticRegression;
import org.apache.mahout.ep.State;
import org.apache.mahout.math.DenseVector;
import org.apache.mahout.math.Vector;

import com.google.common.base.Charsets;
import com.google.common.base.Splitter;
import com.google.common.collect.Iterables;
import com.google.common.collect.Lists;
import com.google.common.io.Files;

/**
 * Hello world!
 *
 */
public class App extends Thread {
	String args[];
	static List<String> lines;
	static AdaptiveLogisticRegression lr;
	List<Integer> myTrain;
	static ArrayList<Vector> oDataset = new ArrayList<Vector>();
	static ArrayList<Integer> oTarget = new ArrayList<Integer>();

	App(List<Integer> train) {
		myTrain = train;
		// System.out.println("Creating " + threadName );
	}

	@Override
	public void run() {
		//System.out.println("Hello World");
		for (int k : myTrain) {
			lr.train(oTarget.get(k), (Vector) oDataset.get(k));
		}
	}

	public static void main(String args1[]) {
		// TODO Auto-generated method stub

		try {
			lines = Files.readLines(new File("data/final.csv"), Charsets.UTF_8);
		} catch (Exception e) {
			return;
		}

		Splitter onComma = Splitter.on(",");
		// List raw =
		// Resources.readLines(App.class.getResource("/data/dataset.csv"),
		// Charsets.UTF_8);

		int i = 0;

		ArrayList<Integer> indices = new ArrayList<Integer>();
		int corpuse_size = lines.size();
		int index = 0;
		for (String current : lines) {
			// System.out.println(current);
			/*
			 * format: rainfall | Snow | Wind Speed | Target variable
			 */

			Iterable<String> current_split = onComma.split(current);
			Vector input_data = new DenseVector(Iterables.size(current_split));
			input_data.set(0, 1);
			int x = 1;
			for (String value : Iterables.limit(current_split, 3)) {
				input_data.set(x++, Double.parseDouble(value));
			}
			oDataset.add(input_data);
			oTarget.add(Integer.parseInt(Iterables.get(current_split, 3)));
			indices.add(index++);
		}

		Random random = RandomUtils.JVM_RANDOM;
		Collections.shuffle(indices, random);
		int train_last_index = (int) (0.75 * corpuse_size);
		List<Integer> train = indices.subList(0, train_last_index);
		List<Integer> test = indices.subList(train_last_index, corpuse_size);

		System.out.println("training");
		lr = new AdaptiveLogisticRegression(2, 4, new L2(1));
		ArrayList<Thread> oThreadList = new ArrayList<Thread>();
		int cores = Runtime.getRuntime().availableProcessors();
		System.out.println(cores);
		for (int pass = 0; pass < cores; pass++) {
			Collections.shuffle(train, random);

			List<Integer> currTrain = new ArrayList(train);
			App oThread = new App(currTrain);
			oThreadList.add(oThread);
			oThread.run();

		}

		for (int x = 0; x < oThreadList.size(); x++)
			try {
				oThreadList.get(i).join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		lr.close();
		org.apache.mahout.ep.State<AdaptiveLogisticRegression.Wrapper, CrossFoldLearner> best = lr
				.getBest();
		CrossFoldLearner state = best.getPayload().getLearner();

		int x = 0;
		for (Integer k : test) {
			int r = state.classifyFull(oDataset.get(k)).maxValueIndex();
			x += r == oTarget.get(k) ? 1 : 0;
		}
		// System.out.println("the correct guesses were: "+x+" out of "+test.size());

		System.out.println();
		float accuracy = (float) (100.0 * x / test.size());
		System.out.println("The accuracy of the logistic regression is: "
				+ accuracy);

	}

}
