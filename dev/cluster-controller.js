// ## Implicit: ##
// This code isn't part of a CusterController.js template,
// it is injected by the application core when the script is loaded.
import cluster from 'core/controllers/ClusterController.js';
import IO from 'core/controllers/InputOutputController.js';

// ## User Code: ##
// This code is the controller template itself,
// purely defined by the user in the ClusterController.js file
import validator from 'CompanyX/ECommerce/ScheduleValidator.js';

cluster
	.on('CompanyX.ECommerce.NewOrder', (event) => {
		let fails = validator.check(event.data)

		if (0 === fails.length) {
			cluster.dispatch(event);
		} else {
			throw NewOrderException(event, 500, fails);
		}
	})
	.then((event) => {
		IO.reply(event);
	})
	.catch((exception) => {
		IO.log(exception, IO.LogLevel.Exception);
		IO.reply(exception);
	});

cluster
	.on('CompanyX.ECommerce.ScheduleOrder', (event) => {
		event.priority = event.priorities.lowest;

		cluster.dispatch(event);
	})
	.then((event) => {
		IO.reply(event);
	})
	.catch((exception) => {
		IO.log(exception, IO.LogLevel.Exception);
		IO.reply(exception);
	});

cluster
	.on('CompanyX.ECommerce.DispatchToDestinatary', (event) => {
		event.priority = event.priorities.higher;

		cluster.dispatch(event);
	})
	.then((event) => {
		IO.reply(event);
	})
	.catch((exception) => {
		IO.log(exception, IO.LogLevel.Exception);
		IO.reply(exception);
	});