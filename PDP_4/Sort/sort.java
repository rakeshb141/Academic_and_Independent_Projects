package com.predictionmarketing.RecommendApp;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

import com.predictionmarketing.RecommendApp.Pa_4.Map;
import com.predictionmarketing.RecommendApp.Pa_4.Reduce;

public class sort {
	// public static int problemSize;

	public static class Map extends
			Mapper<LongWritable, Text, Text, FloatWritable> {

		// private final static IntWritable one = new IntWritable(problemSize);
		// private Text text = new Text();

		public void map(LongWritable key, Text value, Context context) {

			String a[];
			a = value.toString().split(" ");
			float number = Float.parseFloat(a[0].toString());
			int p_Size = Integer.parseInt(a[1].toString());
			int max_no = p_Size * 10;
			int no_of_buckets = 10;
			int limit = max_no / no_of_buckets;
			int int_num = (int) number;
			int bucket_no = int_num / limit;

			try {
				context.write(new Text("" + bucket_no), new FloatWritable(
						number));
			} catch (IOException e) {
				// TODO Auto-generated catch block
				System.out.println("here in Map IO exception");
				e.printStackTrace();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("here in Map Inter exception");
				e.printStackTrace();
			}

		}
	}

	public static class Reduce extends
			Reducer<Text, FloatWritable, Text, FloatWritable> {
		public void reduce(Text key, Iterable<FloatWritable> values,
				Context context) throws IOException, InterruptedException {
			float out = -1;
			ArrayList<Float> array = new ArrayList<Float>();
			for (FloatWritable val : values) {
				out = val.get();
				array.add(out);
				// context.write(key, new FloatWritable(out));
			}
			Collections.sort(array);
			// Log l = LogFactory.getLog(Reduce.class);

			for (int j = 0; j < array.size(); j++) {
				context.write(new Text(""), new FloatWritable(array.get(j)));
				// System.out.println(array.get(j));
				// l.info(array.get(index));

			}
		}
	}

	public static void main(String args[]) {
		try {
			Configuration conf = new Configuration();
			Job job = new Job(conf, "sort");

			long start = System.currentTimeMillis();
			job.setMapperClass(Map.class);
			job.setCombinerClass(Reduce.class);
			job.setReducerClass(Reduce.class);

			job.setOutputKeyClass(Text.class);
			job.setOutputValueClass(FloatWritable.class);

			job.setInputFormatClass(TextInputFormat.class);
			job.setOutputFormatClass(TextOutputFormat.class);

			FileInputFormat.addInputPath(job, new Path(args[0]));
			FileOutputFormat.setOutputPath(job, new Path(args[1]));

			job.setJarByClass(sort.class);
			job.waitForCompletion(true);

			FileSystem fs = FileSystem.get(conf);
			FileStatus[] fstatus = fs.listStatus(new Path(args[1]));

			for (FileStatus status : fstatus) {

				Path path = status.getPath();
				// System.out.println("path :" + path);
				// System.out.println(path.getName());
				if (fs.isFile(path) && path.toString().contains("part")) {
					// System.out.println("IS A FILE SO READING from path\n"+
					// path.toString());
					FSDataInputStream dis = fs.open(path);
					//System.out.println(dis.available());
					byte[] buff = new byte[1024];

					int l;
					while ((l = dis.read(buff)) != -1) {

						System.out.print(new String(buff, 0, l, "UTF-8"));
					}

				}
			}

			long end = System.currentTimeMillis();
			System.out.println("TIME TAKEN : " + (end - start) / 1000.0);
			
			System.out.println("DONE");

		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("here in IP exception");
			e.printStackTrace();
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			System.out.println("here in Interrupted exception");
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			System.out.println("here in ClassNotFound exception");
			e.printStackTrace();
		}

	}
}
