var randomScalingFactor = function(){ return Math.round(Math.random()*100)};
var lineChartData = {
	labels : ["1","2","3","4","5","6","7"],
	datasets : [
		{
			label: "My First dataset",
			fillColor : "rgba(0, 44, 36, 0.)",
			fillColor : "rgba(254, 44, 36, 0.3)",
			strokeColor : "rgba(254, 44, 36, 0.7)",
			pointColor : "rgba(255, 44, 44, 0.9)",
			pointStrokeColor : "#fff",
			pointHighlightFill : "#fff",
			pointHighlightStroke : "rgba(220,220,220,1)",
			data : [randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor()]
		},
		{
			label: "My Second dataset",
			fillColor : "rgba(151,187,205,0.2)",
			strokeColor : "rgba(151,187,205,1)",
			pointColor : "rgba(151,187,205,1)",
			pointStrokeColor : "#fff",
			pointHighlightFill : "#fff",
			pointHighlightStroke : "rgba(151,187,205,1)",
			data : [randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor(),randomScalingFactor()]
		}
	]

};

window.onload = function(){
	var ctx = document.getElementById("canvas").getContext("2d");
	window.myLine = new Chart(ctx).Line(lineChartData, {
		responsive: true
	});
};