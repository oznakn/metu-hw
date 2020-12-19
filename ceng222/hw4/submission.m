% pkg load statistics

N = 1504; % monte carlo simulation size with alpha = 0.02 and error = 0.03
lambda = 160; % given lambda value

p_a = 0.012; % given p value for part a
p_b = 0.79; % given p value for part b

% arrays for keeping the track of results of monte carlo simulations
resultsX_a = zeros(N, 1);
resultsY_a = zeros(N, 1);
resultsX_b = zeros(N, 1);
resultsY_b = zeros(N, 1);

for i=1:N; % monte corla simulations
  m = poissrnd(lambda); % get the number of distinct good types for this day
  total_triplets = (m*(m-1)*(m-2))/6; % total number of triplets

  G = triu(rand(m), 1); % get only upper triangular part of the randomized matrix
  G = (G + G'); % copy the upper part to lower

  G_a = (G<p_a); % calculate bernoulli trials
  G_a = G_a - diag(diag(G_a)); % make the diagonals zero
  G_b = (G<p_b); % calculate bernoulli trials
  G_b = G_b - diag(diag(G_b)); % make the diagonals zero

  X_a = trace(G_a^3)/6; % calculate possible number of triplets for part a
  X_b = trace(G_b^3)/6; % calculate possible number of triplets for part b

  % save the both the possible number of triples and the ratio of
  % possible number of triplets to total triplets to arrays
  resultsX_a(i) = X_a;
  resultsY_a(i) = X_a/total_triplets;
  resultsX_b(i) = X_b;
  resultsY_b(i) = X_b/total_triplets;
end;

% estimate the probabilities
prob_a = mean(resultsX_a<=1)
prob_b = mean(resultsY_b>0.5)

% estimate values
X_a = mean(resultsX_a)
X_b = mean(resultsX_b)

Y_a = mean(resultsY_a)
Y_b = mean(resultsY_b)

% calculate Std of the monte carlo study
std_X_a = std(resultsX_a)
std_Y_a = std(resultsY_a)
std_X_b = std(resultsX_b)
std_Y_b = std(resultsY_b)
